#!/bin/bash

# monitoring.sh: collects system info and broadcasts it to every open
# terminal. Run by root's cron every 10 minutes (sudo crontab -e).
#
# SHELL SYNTAX USED
#   #!/bin/bash    first line: run this file with bash
#   var=$(cmd)     run cmd and store what it output in var
#   a | b          pipe: what a outputs becomes b's input
#   $var ${var}    the value of var (braces when text follows the name)
#   "..."          text where $var is replaced by its value
#   '...'          text kept exactly as written ($ is not replaced)
#
# GENERAL COMMANDS
#   grep "x"       keep only the lines containing x
#     -c           count matching lines
#     -v           keep lines NOT containing x
#     "^x"         ^ = x must be at the start of the line
#   awk '...'      process text column by column: $1 is the 1st column,
#                  $2 the 2nd...; '$1 == "a" {...}' runs {...} only on
#                  lines whose 1st column is a
#   wc -l          count lines
#   sort -u        sort lines and drop duplicates (u = unique)
#   tail           print the last lines of its input
#   printf         print with a format: %s text, %d whole number,
#                  %.1f number with 1 decimal, \n new line
#
# FILES READ
#   /proc/cpuinfo           not a real file: the kernel generates it on
#                           the fly; one block of lines per virtual CPU
#   /var/log/sudo/sudo.log  sudo's log, set by "Defaults logfile=" in
#                           /etc/sudoers.d/; one entry per sudo command
#
# FILES WRITTEN
#   /dev/pts/N              one file per SSH terminal; writing into it
#                           prints on that screen

# ARCH
# all system info (kernel name, hostname, kernel version, etc.)
arch=$(uname -a)

# CPU PHYSICAL
# /proc/cpuinfo has a "physical id" line per virtual CPU telling which
# physical chip it belongs to; sort -u keeps one line per chip, wc -l
# counts them
cpu_physical=$(grep "physical id" /proc/cpuinfo | sort -u | wc -l)

# CPU VIRTUAL
# each virtual CPU starts with a "processor : N" line; count them
cpu_virtual=$(grep -c "^processor" /proc/cpuinfo)

# RAM
# free -m: memory usage in MB (reads /proc/meminfo);
# on the "Mem:" line, column 2 is total and column 3 is used
ram_total=$(free -m | awk '$1 == "Mem:" {print $2}')
ram_use=$(free -m | awk '$1 == "Mem:" {print $3}')
# used / total * 100, printed with 2 decimals
ram_percent=$(free -m | awk '$1 == "Mem:" {printf("%.2f"), $3/$2*100}')

# DISK
# df -m: one line per mounted filesystem, sizes in MB (column 2 = size,
# column 3 = used)
# grep /dev/ keeps real disks,
# grep -v /boot drops the boot partition,
# awk adds up size (t) and used (u) over all lines;
# END runs once at the end: total in GB (t/1024), used in MB, used percent
# read -r a b c <<< "x y z" splits the three results into three variables
read -r disk_total disk_use disk_percent <<< $(df -m | grep /dev/ | grep -v /boot | awk '{t += $2; u += $3} END {printf "%.1fGb %dMb %d", t/1024, u, (u/t)*100}')

# CPU LOAD
# top -bn2: plain-text output, 2 samples (the 1st averages since boot,
# the 2nd is current); tail -1 keeps it
# sed: top drops the space in "ni,100.0" when a field is exactly 100.0,
# shifting the columns — this puts it back (& = the matched character)
# $8 is idle %, so load = 100 - idle
cpu_load=$(top -bn2 | grep '%Cpu(s)' | tail -1 | sed 's/[:,]/& /g' | awk '{printf "%.1f", 100 - $8}')

# LAST BOOT
# who -b prints "system boot 2026-09-17 00:12";
# keep date ($3) and time ($4)
last_boot=$(who -b | awk '$1 == "system" {print $3 " " $4}')

# LVM USE
# lsblk: lists disks, partitions and volumes with their TYPE;
# if [ N -gt 0 ]: if N is greater than 0;
# count the lvm lines, answer yes or no
lvm_use=$(if [ $(lsblk | grep "lvm" | wc -l) -gt 0 ]; then echo yes; else echo no; fi)

# ACTIVE CONNEXIONS
# ss: lists network connections (sockets); -t = TCP only,
# state established = only open, active connections
# tail -n +2 starts from line 2, skipping the header, then count
tcp_conex=$(ss -t state established | tail -n +2 | wc -l)

# USER LOG
# who: one line per login session: user, terminal, date
# who lists the console twice on Debian 13 (seat0 and tty1), so skip seat lines
# !~ /^seat/: column 2 does NOT start with "seat"
user_sessions=$(who | awk '$2 !~ /^seat/' | wc -l)
# keep only the user name (column 1), once per user, then count
user_log=$(who | awk '{print $1}' | sort -u | wc -l)
# same names, joined on one line: paste -s = join lines, -d ' ' = with spaces
user_names=$(who | awk '{print $1}' | sort -u | paste -sd ' ')

# NETWORK
# hostname -I: all IP addresses of the machine; keep the first (IPv4)
ip=$(hostname -I | awk '{print $1}')
# ip link: network interfaces; the MAC address is on the "link/ether"
# line, column 2
mac=$(ip link | grep "link/ether" | awk '{print $2}')

# SUDO
# every command run with sudo adds an entry with "COMMAND=" to sudo's
# log file; count those lines
sudo_count=$(grep -c "COMMAND=" /var/log/sudo/sudo.log)

# MESSAGE
# all results in one text; the leading tabs indent each line
msg="	Architecture: $arch
	CPU physical: $cpu_physical
	vCPU: $cpu_virtual
	Memory Usage: $ram_use/${ram_total}MB ($ram_percent%)
	Disk Usage: $disk_use/${disk_total} ($disk_percent%)
	CPU load: $cpu_load%
	Last boot: $last_boot
	LVM use: $lvm_use
	Connections TCP: $tcp_conex ESTABLISHED
	User log: $user_log [$user_names] ($user_sessions sessions)
	Network: IP $ip ($mac)
	Sudo: $sudo_count cmd"

# BROADCAST
# wall reaches tty1; on Debian 13 it misses SSH sessions, so write to each pts
# whoami: current user (root under cron); hostname: machine name;
# date '+...': current date
header="Broadcast message from $(whoami)@$(hostname) ($(date '+%a %b %e %H:%M:%S %Y')):"

# wall: writes a message to every logged-in terminal;
# -n = no banner of its own (root only), we print our header instead
wall -n "
$header

$msg"

# for ... do ... done: repeat for every file matching /dev/pts/[0-9]*
# (pts/0, pts/1...; skips /dev/pts/ptmx, which is not a terminal)
# > "$term" writes into that terminal; 2>/dev/null comes first so an
# error (a session closing mid-loop) is thrown away, not printed
for term in /dev/pts/[0-9]*
do
      printf "\n%s\n\n%s\n" "$header" "$msg" 2>/dev/null > "$term"
done
