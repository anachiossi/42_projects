# Born2beRoot — BUILD GUIDE (login: `anade-mo` → hostname `anade-mo42`)

**How to use this:** each step is `CONCEPT` (learn first, plain words + correct terms) → `DO` (exact clicks/commands). Don't skip the concept — you have to *explain* all of this at the defense.

> Language note: I wrote this in English because your message and both PDFs are English and every command is English anyway. Your stored convention says defense prep in Italian — say the word and I'll flip the spoken-answer parts of the **defense guide** to Italian.

---

## 3 RULES THAT CAN GIVE YOU A 0 — read before touching anything

1. **No graphical interface.** No X.org, no Wayland, no desktop. Grade 0 if present. During install you will *uncheck* the desktop.
2. **The `.vdi` signature must match `signature.txt` at defense.** The hash changes every time you boot the VM. So: final shutdown → compute hash → paste → **never boot again** until defense.
3. **Moving Windows → school Ubuntu:** copy the **raw VM folder** (the `.vdi` file byte-for-byte). Do **NOT** "Export Appliance" to `.ova` and re-import — that recompresses the disk and changes the hash → 0. (Detail in Phase 6.)

---

## Time plan (12h total, ~4h today)

- **Today (Phase 1, ~4h):** VirtualBox + Debian install with encrypted LVM + user/hostname/groups. This is the slow, first-time-scary part. Get here and today is a win.
- **Session 2 (~4h):** sudo rules → password policy → SSH → UFW → AppArmor check.
- **Session 3 (~4h):** `monitoring.sh` + cron → README → signature → git → defense rehearsal.

---

# PHASE 0 — Windows gotcha (2 min, do this first)

`CONCEPT` — WSL2 uses **Hyper-V** (Windows' own virtualization layer). VirtualBox and Hyper-V historically fight over the CPU's virtualization hardware. VirtualBox 7 *runs* with Hyper-V on, but sometimes slowly (you'll see a green turtle icon). Fine for this project — it's a tiny server. Just don't panic if it warns you.

`DO` — Nothing yet. Just know: if the VM won't start or is unbearably slow, that's the Hyper-V clash, and the fix is a separate conversation. Proceed normally for now.

---

# PHASE 1 — Get the OS running

## Step 1.1 — What is a VM / why VirtualBox

`CONCEPT` — A **virtual machine (VM)** is a whole computer *emulated in software*: it has its own virtual disk, CPU, RAM, and OS, all living as files on your real machine (the **host**). The software that creates and runs VMs is a **hypervisor**. **VirtualBox** is a free, cross-platform *type-2 hypervisor* (runs on top of your normal OS). The subject *mandates* VirtualBox (or UTM only if you can't run VirtualBox). Why VMs: you can install/break/reset a full OS without risking your real machine — perfect for practicing sysadmin.

`DO` —
1. Download: **https://www.virtualbox.org/wiki/Downloads** → "Windows hosts".
2. Install with defaults (it'll blip your network briefly — normal).

## Step 1.2 — Get Debian

`CONCEPT` — **Debian** is a Linux distribution: extremely stable, community-run, the reference base many distros build on. The subject recommends it for beginners. You want the **latest stable** (no testing/unstable) and the **netinst** ISO — a small installer that pulls packages from the internet during install. An **ISO** is a disk image (a bootable CD in a file).

`DO` —
1. Download page: **https://www.debian.org/distrib/** → "small installation image (netinst)" → **amd64**. (Currently Debian 13 "Trixie".)
2. Save the `.iso` somewhere you'll find it.

## Step 1.3 — Create the VM shell

`CONCEPT` — You're defining the *hardware* of your fake computer before installing anything on it. Modest specs: this is a headless server.

`DO` — In VirtualBox → **New**:
- Name: `anade-mo42` · Type: Linux · Version: Debian (64-bit)
- Attach the ISO now if it asks; **uncheck "Skip Unattended Installation"** stays checked-off — you want to install manually, so tick *Skip Unattended Installation* so it doesn't auto-configure.
- RAM: **1024–2048 MB** · CPUs: **1**
- Disk: **create VDI**, dynamically allocated, **~30 GB** (leaves room for bonus later)
- After creation → **Settings → Network → Adapter 1 → NAT** (default) → expand **Advanced → Port Forwarding** → add a rule:
  `Name: ssh · Protocol: TCP · Host Port: 4242 · Guest Port: 4242` (leave IPs blank).
  This is what lets you SSH into the VM from Windows/WSL later.

## Step 1.4 — Encrypted LVM partitions (the heart of the install)

`CONCEPT` — Three ideas stacked:
- **Partition** = a labeled slice of a disk.
- **LVM (Logical Volume Manager)** = a flexible layer *on top of* partitions. Instead of rigid slices, you pool space into a **Volume Group (VG)** and carve flexible **Logical Volumes (LV)** you can resize later. Think: partitions are fixed drawers; LVM is one big drawer you re-divide anytime.
- **Encryption (LUKS)** = the disk contents are scrambled; without the passphrase at boot it's unreadable. Protects data if the machine is stolen.
- Subject wants **≥2 encrypted partitions using LVM**. `/boot` must stay *unencrypted* (the machine has to read it before it knows the passphrase). Everything else goes inside one encrypted container that holds the LVM.

Target layout (mandatory): `sda1 → /boot` (plain) · `sda5 → encrypted → VG "LVMGroup" → root(/), swap, home(/home)`.

`DO` — Boot the VM, choose **Install** (text) or **Graphical Install**. Set language/keyboard, hostname **`anade-mo42`**, domain blank. Set root password + create user `anade-mo` (you'll rewrite all passwords later). At **Partition disks**:
1. Method: **Manual**.
2. Select the disk → **Yes** (new empty partition table).
3. Select **FREE SPACE** → Create new partition → **500 MB** → Primary → Beginning → *Use as* **Ext4**, *Mount point* **/boot** → Done.
4. Select **FREE SPACE** → Create new partition → **all remaining** → Primary → *Use as* **physical volume for encryption** → Done.
5. **Configure encrypted volumes** → **Yes** (write) → Create → pick that partition → Finish → set a **passphrase** (twice). *(It may "erase with random data" — can be slow; let it run.)*
6. **Configure the Logical Volume Manager** → Yes → **Create volume group** → name **`LVMGroup`** → select the encrypted device (`.../sda5_crypt`).
7. **Create logical volume** ×3 in `LVMGroup`:
   - `root` → **10 GB**
   - `swap` → **2.4 GB**
   - `home` → **all the rest**
   → Finish.
8. Back in the list, assign each LV:
   - `LVMGroup-root` → *Use as* Ext4, mount **/**
   - `LVMGroup-swap` → *Use as* **swap area**
   - `LVMGroup-home` → Ext4, mount **/home**
9. **Finish partitioning and write changes** → **Yes**.

## Step 1.5 — Software selection (DO NOT install a desktop)

`CONCEPT` — **tasksel** is the "what should this machine be" menu. A desktop environment = a GUI = instant 0. A server needs almost nothing.

`DO` — At the **Software selection** screen, use **space** to toggle so that:
- ❌ **UNCHECK** "Debian desktop environment" and any GNOME/KDE line
- ✅ CHECK **SSH server**
- ✅ CHECK **standard system utilities**
Continue. Install **GRUB** to the disk (`/dev/sda`) when asked. Finish → reboot → remove the ISO (Devices/Settings) so it boots from disk.

## Step 1.6 — First login, sudo, groups, hostname

`CONCEPT` —
- **sudo** = "do this one command as another user (usually root)". Lets a normal user run admin tasks without living as root. Safer: logged, revocable, per-command.
- **Groups** bundle permissions. The subject wants your user in **`sudo`** (admin rights) and **`user42`** (a group the project requires you to create).

`DO` — Log in as **root** (you don't have sudo yet). Then:
```bash
apt update && apt upgrade -y
apt install -y sudo
groupadd user42                 # create the required group (skip if exists)
usermod -aG sudo,user42 anade-mo
# verify:
getent group sudo user42        # anade-mo should appear in both
hostname                        # must print: anade-mo42
```
If hostname is wrong: `hostnamectl set-hostname anade-mo42` and make `/etc/hosts` line match. Log out of root, log in as **anade-mo**, test `sudo whoami` → should print `root`.

**✅ End of today's target.** OS runs, encrypted LVM confirmed with `lsblk`, user is in `sudo` + `user42`, hostname correct. If you got here in 4h, that's exactly on plan.

---

# PHASE 2 — Security config (Session 2)

> **Editor: vim.** `visudo` and `crontab -e` open whatever the system calls
> the default editor. To make that vim once and for all:
> ```bash
> sudo update-alternatives --set editor /usr/bin/vim.basic
> echo 'export EDITOR=vim' >> ~/.bashrc
> ```
> **vim survival kit:** `i` start typing · `Esc` stop typing · `:w` save ·
> `:wq` save and quit · `:q!` quit throwing changes away · `/text` search ·
> `dd` delete the line · `u` undo. In vim you are in command mode until you
> press `i`; that is why typing does nothing at first.

## Step 2.1 — sudo strict rules

`CONCEPT` — You harden *how* sudo behaves: limit password retries, log everything, force a real terminal, and lock down which directories sudo will search for commands (**secure_path**, so nobody sneaks a malicious `ls` from a weird folder). Never edit sudoers with a plain editor — **`visudo`** checks syntax so a typo can't lock you out.

`DO` —
```bash
sudo mkdir -p /var/log/sudo
sudo visudo -f /etc/sudoers.d/born2beroot
```
Put in (understand each line — defense asks):
```
Defaults        passwd_tries=3
Defaults        badpass_message="Wrong password. Think again, Dexter."
Defaults        logfile="/var/log/sudo/sudo.log"
Defaults        log_input,log_output
Defaults        iolog_dir="/var/log/sudo"
Defaults        requiretty
Defaults        secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin"
```
- `passwd_tries=3` → 3 attempts. `badpass_message` → custom fail message. `logfile` + `log_input,log_output` + `iolog_dir` → logs commands *and* their I/O to `/var/log/sudo/`. `requiretty` → TTY mode. `secure_path` → restricted command paths.
Test: run any `sudo` command, then `sudo cat /var/log/sudo/sudo.log` — it should have grown.

## Step 2.2 — Password policy

`CONCEPT` — Two halves:
- **Aging** (how long passwords live) lives in `/etc/login.defs` for *future* users; for users that *already exist* you also apply it with **`chage`**.
- **Strength** (length, character types, etc.) is enforced by a **PAM** module called **pwquality** at password-change time. **PAM** = the pluggable system Linux uses to decide "is this authentication allowed?".

`DO` — Aging in `/etc/login.defs` (edit these lines):
```
PASS_MAX_DAYS   30
PASS_MIN_DAYS   2
PASS_WARN_AGE   7
```
Apply to existing accounts:
```bash
sudo chage -M 30 -m 2 -W 7 anade-mo
sudo chage -M 30 -m 2 -W 7 root
sudo chage -l anade-mo          # verify
```
Strength:
```bash
sudo apt install -y libpam-pwquality
sudo vim /etc/pam.d/common-password
```
Find the `pam_pwquality.so` line and make it read (one line):
```
password requisite pam_pwquality.so retry=3 minlen=10 ucredit=-1 lcredit=-1 dcredit=-1 maxrepeat=3 reject_username difok=7 enforce_for_root
```
Meaning: `minlen=10` (≥10 chars) · `ucredit/lcredit/dcredit=-1` (≥1 upper, lower, digit) · `maxrepeat=3` (no >3 identical in a row) · `reject_username` (can't contain the name) · `difok=7` (≥7 new chars vs old — subject exempts root; `difok` is skipped for root by default) · `enforce_for_root` (root must also obey the rest).

**Then rewrite ALL passwords** (subject requires it, and old ones predate the policy):
```bash
sudo passwd anade-mo
sudo passwd root
```

## Step 2.3 — SSH on 4242, no root login

`CONCEPT` — **SSH** = encrypted remote shell; you log into the machine over the network. Moving it off default port 22 → 4242 cuts noise/bots. Disabling root-over-SSH means an attacker must first know a *user* name, then still escalate.

`DO` —
```bash
sudo vim /etc/ssh/sshd_config
```
Set:
```
Port 4242
PermitRootLogin no
```
Then:
```bash
sudo systemctl restart ssh
sudo ss -tlnp | grep 4242        # should show sshd listening on 4242
```
Test from Windows/WSL (thanks to the port-forward rule): `ssh anade-mo@localhost -p 4242`.
*Gotcha:* if the port didn't change, some setups have socket activation — check `sudo systemctl status ssh` and, if present, `/lib/systemd/system/ssh.socket`. Ask me if you hit this.

## Step 2.4 — UFW firewall, only 4242

`CONCEPT` — A **firewall** decides which network ports are reachable. **UFW** ("Uncomplicated FireWall") is Debian's friendly front-end. You block everything, then open only 4242.

`DO` —
```bash
sudo apt install -y ufw
sudo ufw enable                  # also enables it on boot
sudo ufw allow 4242
sudo ufw status numbered         # 4242 ALLOW; nothing else
```

## Step 2.5 — Confirm AppArmor

`CONCEPT` — **AppArmor** confines programs to a profile of what files/actions they're allowed — damage control if an app is compromised. On Debian it's on by default; you just prove it's running.

`DO` —
```bash
sudo aa-status                   # should say profiles loaded
systemctl is-active apparmor     # active
```

---

# PHASE 3 — monitoring.sh (Session 3)

`CONCEPT` — A **bash script** is a text file of shell commands. Yours must broadcast server stats to **every** open terminal (`wall`) at boot and every 10 min (**cron**). At the defense you must *read your own code aloud and explain it*, then *stop it without editing the file*. So **write this yourself** — I'll give you the tool for each line, not the finished script. That's also the 42 rule (no copy-paste answers) and it's the only way you pass the defense.

`DO` — Build `monitoring.sh` using these building blocks (one variable each):

| Needs to show | Tool to get it |
|---|---|
| Architecture + kernel | `uname -a` |
| Physical CPUs | `grep "physical id" /proc/cpuinfo \| sort -u \| wc -l` |
| Virtual CPUs | `grep -c "^processor" /proc/cpuinfo` |
| RAM used/total + % | parse `free -m` with `awk` |
| Disk used/total + % | sum real filesystems from `df -m` (or `df -BM`) with `awk` |
| CPU load % | `top -bn1` → grab idle → `100 - idle` (awk/printf) |
| Last boot | `who -b`  (or `uptime -s`) |
| LVM active? | count lvm lines: `lsblk \| grep -c lvm` → yes/no |
| Active connections | `ss -t state established \| tail -n +2 \| wc -l` |
| Logged-in users | `who \| wc -l`  (or `users` unique count) |
| IP + MAC | `hostname -I` and `ip link show` (the `link/ether` line) |
| Sudo command count | `grep -c COMMAND /var/log/sudo/sudo.log` |

Then broadcast: build your message into a variable and end the script with something like `wall "$message"` (or `echo "$message" | wall`). Make it runnable:
```bash
sudo mv monitoring.sh /usr/local/bin/
sudo chmod +x /usr/local/bin/monitoring.sh
```
Schedule with cron (**cron** = the Linux job scheduler; `*/10 * * * *` = "every 10 min"):
```bash
sudo crontab -e
```
Add:
```
*/10 * * * * /usr/local/bin/monitoring.sh
```
- **Defense trick — run every minute:** change `*/10` to `* * * * *`.
- **Defense trick — stop WITHOUT editing the script:** comment out that crontab line (`sudo crontab -e`, put `#` in front) or `sudo systemctl stop cron`. The `.sh` file stays untouched — that's the point.

---

# PHASE 4 — README.md

`CONCEPT` — The README is graded. Miss a required piece and the defense *ends immediately*. It documents *your choices*, not generic theory.

`DO` — Create `README.md`. **First line must be italic, exactly:**
```
*This project has been created as part of the 42 curriculum by anade-mo.*
```
Required sections: **Description** (goal + overview) · **Instructions** (how to run/evaluate) · **Resources** (references + *how you used AI: which tasks, which parts*) · **Project description** with your OS choice + pros/cons, your design choices (partitioning, password policy, user mgmt, services), and these four comparisons: **Debian vs Rocky**, **AppArmor vs SELinux**, **UFW vs firewalld**, **VirtualBox vs UTM**. (Short answers for all four are in the defense guide — put them in your own words.)

---

# PHASE 5 — signature + git

`CONCEPT` — You submit **only** `README.md` and `signature.txt`. Never the VM, never passwords. The signature is a **SHA-1 hash** — a fingerprint of the `.vdi` file's exact bytes. Boot the VM and the bytes change → fingerprint changes → mismatch → 0.

`DO` (in order, this exact sequence):
1. **Shut down** the VM cleanly. Do not boot it again.
2. Get the hash of the `.vdi`:
   - Windows: `certUtil -hashfile "anade-mo42.vdi" sha1`
   - Linux/WSL/school: `sha1sum anade-mo42.vdi`
3. Paste the hash into `signature.txt`.
4. Commit + push `README.md` + `signature.txt` to your 42 git repo. **Nothing else.**

---

# PHASE 6 — Move Windows → school Ubuntu (do this right or it's a 0)

`CONCEPT` — The signature is the `.vdi`'s byte fingerprint. A **raw file copy** keeps the bytes identical → same hash. **Export Appliance (.ova)** *recompresses* the disk → new bytes → new hash → mismatch. So copy, don't export.

`DO` —
1. Shut the VM down. Copy the whole **`VirtualBox VMs/anade-mo42/`** folder (contains `.vdi` + `.vbox`) to USB/cloud.
2. On the school Ubuntu machine: put the folder in that user's `~/VirtualBox VMs/`, then in VirtualBox → **Add** (Machine → Add) the existing `.vbox` — **not** "Import Appliance".
3. Safest: compute the signature **on the machine you'll defend on**, right before the deadline, after final shutdown — then it's guaranteed to match. If you ever boot after hashing, re-hash and re-commit.

---

# SUPPORT FILES — keep these (NOT in git)

Keep a local, private folder (or a Notion page) — never push it:
- `secrets.md` — root password, anade-mo password, LUKS disk passphrase. **Local only.** If you lose the LUKS passphrase the VM is gone.
- `commands_cheatsheet.md` — the verify commands from each phase (for the live defense).
- In the **repo** only: `README.md`, `signature.txt`.

---

## End-of-build checklist
- [ ] `lsblk` shows encrypted LVM (crypt → LVMGroup → root/swap/home)
- [ ] No desktop / GUI installed
- [ ] `anade-mo` in `sudo` + `user42`; hostname `anade-mo42`
- [ ] sudo: 3 tries, custom msg, logs in `/var/log/sudo/`, requiretty, secure_path
- [ ] password policy (aging + pwquality) applied; **all** passwords rewritten
- [ ] SSH on 4242, root login disabled
- [ ] UFW active, only 4242 open
- [ ] AppArmor active
- [ ] `monitoring.sh` runs via cron every 10 min, broadcasts via `wall`
- [ ] README complete (italic first line + 4 comparisons + AI usage)
- [ ] signature.txt = current `.vdi` hash; repo has only README + signature
