# Born2beRoot — DEFENSE GUIDE (what the evaluator checks + your answers)

Follow the eval sheet order. For each section: **the command that proves it** + **what you must explain**. If you can't explain something, that section fails — so rehearse the explanations, not just the commands.

> Say the word and I'll translate the "you say" answers below into Italian for the live defense.

---

## PRE-DEFENSE — before the evaluator arrives
- [ ] **VM is OFF** (evaluator won't accept an already-running VM).
- [ ] **No snapshots exist** (delete any). They'll make a defense snapshot themselves.
- [ ] `signature.txt` in repo **matches** the current `.vdi` hash (`sha1sum`/`certUtil`). If you booted since hashing → re-hash + re-commit.
- [ ] Repo cloned fresh into an **empty folder**; contains only `README.md` + `signature.txt`.
- [ ] No aliases hiding real behavior (`type <cmd>`), README first line italic + all 4 comparisons present.

---

## SECTION-BY-SECTION (command → explanation)

> **Live tasks use vim.** `sudo crontab -e` and `sudo visudo` open vim:
> `i` to type · `Esc` · `:wq` to save and quit · `:q!` to leave without
> saving. To comment the cron line: put the cursor on it, press `i`, type
> `#`, then `Esc` `:wq`.

### OS / VM basics
- OS: `cat /etc/os-release` or `head -n 2 /etc/os-release` → Debian.
- **Say:** a VM is a full computer emulated in software by a hypervisor (VirtualBox); the disk/RAM/CPU are virtual. Purpose: run/isolate/reset an OS safely on one physical host.

### Simple setup
- No GUI: `ls /usr/bin | grep -iE "xorg|wayland"` returns nothing; there's no display manager.
- UFW up: `sudo ufw status` → active. SSH up: `sudo systemctl status ssh`.

### User step 1 & 2 (live tasks)
- Prove your user: `getent group sudo user42` → `anade-mo` in both.
- **They will have you:** create a new user, create group **`evaluating`**, add the user to it:
```bash
sudo adduser <newuser>
sudo groupadd evaluating
sudo usermod -aG evaluating <newuser>
getent group evaluating          # verify membership
```
- **Explain password policy pros/cons** (not "because the subject said so"):
  - *Pros:* forces strong, rotating, non-reused passwords → much harder to brute-force or reuse a leaked one.
  - *Cons:* frequent forced changes annoy users → weaker/written-down passwords, more lockouts and helpdesk load; modern guidance (NIST) leans toward length over forced rotation.

### Hostname & partitions (live task)
- View partitions: `lsblk` → compare to the subject's example (crypt → LVMGroup → root/swap/home).
- **They will have you** change the hostname to *their* login and reboot:
```bash
sudo hostnamectl set-hostname <peer42>
# also fix /etc/hosts, then:
sudo reboot
```
  After reboot the prompt shows the new name. Then restore yours: `sudo hostnamectl set-hostname anade-mo42`.
- **Explain LVM:** a flexible layer over partitions — pool disks into a Volume Group, carve resizable Logical Volumes; resize/add space without repartitioning. Encryption (LUKS) sits under it so data at rest is unreadable without the passphrase.

### SUDO
- Installed: `dpkg -l | grep sudo`.
- Show your rules: `sudo cat /etc/sudoers.d/born2beroot`.
- Log folder: `sudo ls -l /var/log/sudo/` (≥1 file); run a `sudo` command, show the log grew.
- **Explain sudo:** run a command as another user (default root) with per-command auth + logging, instead of being root full-time. Walk each Defaults line: passwd_tries, badpass_message, logfile+log_input/output, requiretty (TTY mode), secure_path.

### UFW (live task)
- `sudo ufw status numbered` → 4242 only.
- **They will have you** open then remove 8080:
```bash
sudo ufw allow 8080
sudo ufw status numbered         # 8080 now listed
sudo ufw delete allow 8080       # or: sudo ufw delete <number>
sudo ufw status numbered         # 8080 gone
```
- **Explain UFW:** simplified front-end to the kernel's netfilter firewall; default-deny inbound, you explicitly allow ports.

### SSH
- `sudo ss -tlnp | grep 4242` → sshd on 4242 only. `sudo grep -E "Port|PermitRootLogin" /etc/ssh/sshd_config`.
- **They will** create a new account and log in over SSH: `ssh <newuser>@<ip> -p 4242`. Then confirm **root is refused**: `ssh root@<ip> -p 4242` → denied.
- **Explain SSH:** encrypted remote login over the network; custom port + no-root-login reduce attack surface.

### monitoring.sh (live tasks)
- Show the code: `cat /usr/local/bin/monitoring.sh` and **read it line by line** — explain how each value is fetched.
- **Explain cron:** the time-based job scheduler; `*/10 * * * *` = every 10 minutes; `wall` broadcasts to all terminals.
- **They will have you make it run every minute:** `sudo crontab -e` → `*/10` → `* * * * *`.
- **They will have you stop it WITHOUT editing the script:** comment the crontab line (`#`) or `sudo systemctl stop cron`. The `.sh` file is untouched.
- Final: reboot; confirm script still exists, same path, same permissions (`ls -l`), unmodified.

---

## BONUS SHOWCASE — your WordPress film portfolio

The bonus is only graded if the mandatory part is perfect, so show this **after** the mandatory sections. Full build steps: bonus guide, Part 2 (Step 2.8 for the import).

### Before the evaluator arrives
- [ ] Site content is final **before** the last shutdown + hash: the posts live inside the `.vdi`, so any change after hashing breaks `signature.txt`.
- [ ] VirtualBox port forwarding still has `http · TCP · 8080 → 80` (next to `ssh · 4242 → 4242`).
- [ ] No leftover files: `ls ~` shows no `wp_films/` and no `wp-cli.phar`.
- [ ] Do **not** re-run `import_films.sh` during the defense: it's done; you only explain it.

### The demo, in order

**1. The services are running**
```bash
systemctl is-active lighttpd mariadb        # active, active
sudo ufw status numbered                    # 4242 and 80
dpkg -l | grep -E "apache2|nginx"           # nothing
```
- **Say:** "Port 80 is HTTP inside the VM; VirtualBox forwards Windows' 8080 to it, so the site is at localhost:8080."

**2. The site** — Windows browser → **http://localhost:8080**
- Home page: the film posts with their full pictures, newest shoot first.
- Open one film: description, details list (role, director, production, country, platforms, release), IMDb link.
- Click a category (e.g. **TV Series**): only those films.
- **Say:** "These are the 65 films from my own portfolio website, imported into WordPress."

**3. How a page is built** (the key explanation)
```
browser ─HTTP─▶ lighttpd ─FastCGI─▶ PHP (WordPress) ─SQL─▶ MariaDB
```
- **Say:** "My github.io site is static: finished HTML files. WordPress has no HTML file per page: for every visit, PHP reads the post from MariaDB and the theme wraps it in its layout. The HTML is built on the spot."

**4. The data really is in MariaDB**
```bash
sudo mariadb -e "SELECT COUNT(*) FROM wordpress.wp_posts WHERE post_type='post' AND post_status='publish';"
sudo mariadb -e "SELECT post_title FROM wordpress.wp_posts WHERE post_type='post' ORDER BY post_date DESC LIMIT 5;"
```
- **Say:** "Pictures are stored in the same table as posts, as `attachment` rows. That's why post IDs went up by 2 during the import: one post, one picture."

**5. Live edit (proves it's dynamic)**
- `http://localhost:8080/wp-admin` → log in → **Posts → Add New** → title "Hello from the defense" → **Publish**.
- Reload the home page: the new post is on top.
- Show it in the database:
  ```bash
  sudo mariadb -e "SELECT post_title FROM wordpress.wp_posts WHERE post_title LIKE 'Hello from%';"
  ```
- Delete it afterwards (Posts → Trash) or leave it; either is fine.

**5b. Quick live check from the terminal** (no browser needed)
```bash
cd /var/www/html
id=$(sudo -u www-data wp post create --post_title="Live check $(date +%H:%M:%S)" --post_status=publish --porcelain)
curl -s -H "Host: localhost:8080" http://localhost/ | grep -o "Live check [0-9:]*"
sudo -u www-data wp post delete "$id" --force
```
- Line 2 publishes a post titled with the current time; `--porcelain` stores its ID in `id`.
- Line 3 asks the web server for the home page, the same way a browser does, and finds that title in the HTML. If the time prints, the whole chain answered: lighttpd → PHP → MariaDB → back.
- `-H "Host: localhost:8080"`: WordPress knows itself as `localhost:8080` and would redirect any other address, so curl presents itself with that name.
- Line 4 removes the test post (`--force` skips the trash).
- **Say:** "I created a post, and seconds later the web server serves it: nothing was written to an HTML file, the page is built from the database on every request."

**6. How the films got there (WP-CLI)**
```bash
wp --info | tail -1                                                        # WP-CLI version
cd /var/www/html
sudo -u www-data wp post list --meta_key=film_id --format=count            # 65
sudo -u www-data wp term list category --fields=name,count                 # film types
```
- **Say:**
  - "WP-CLI manages WordPress from the terminal. It runs WordPress's own PHP code, so the posts went into MariaDB exactly as if I had typed them in the admin page."
  - "A Python script on my PC converted the website's `films.json` and AVIF pictures to JPG. A bash script on the VM used `jq` to read each film and `wp post create` / `wp media import` to publish it."
  - "Each post stores a `film_id`, so re-running the import skips films that already exist, and I can delete exactly the imported ones."
  - "The script uses `runuser` instead of `sudo` for the WordPress calls, so the hundreds of calls didn't inflate the sudo count in `monitoring.sh`."
  - "It runs WP-CLI as `www-data` because that's the user lighttpd and PHP run as, and it owns the WordPress files."

**7. The picture fix**
- **Say:** "The theme showed featured images in a fixed shape and cropped them. I changed the display, not the pictures: the featured image's aspect ratio is set to the original (template setting / Additional CSS), so each picture keeps its own proportions."
- If asked, show it: **Appearance → Editor** (template's featured image block, or Styles → Additional CSS).

### Likely questions
| Question | Answer |
|---|---|
| Why lighttpd? | Required by the subject; lightweight, little RAM — good for a small VM. |
| Why `php-cgi` and not `php`? | The `php` package pulls in Apache2, which is forbidden. lighttpd runs PHP through FastCGI with `php-cgi`. |
| Who can reach the database? | Only `wpuser@localhost`, only the `wordpress` database, never from the network (port 3306 isn't open). |
| Where are the files? | `/var/www/html`, owned by `www-data`; the pictures are in `wp-content/uploads`. |
| Why is `/var` its own volume? | The site (`/var/www`) and the database (`/var/lib/mysql`) grow there; if they fill it, `/` keeps working. |

---

## QUICK Q&A — memorize these (say them in your own words)

**apt vs aptitude** — both manage `.deb` packages. `apt` is the modern, simple everyday front-end. `aptitude` is older/interactive with a smarter dependency resolver and can suggest fixes when conflicts arise. **APT** itself = the underlying package system (Advanced Package Tool).

**Debian vs Rocky** — Debian: community-run, `.deb`/apt, uses **AppArmor**, very general-purpose, fast free releases. Rocky: RHEL-compatible rebuild, `.rpm`/dnf, uses **SELinux**, aimed at enterprise/long-term stability. Debian is easier for beginners → why you chose it.

**AppArmor vs SELinux** — both are Mandatory Access Control (confine what programs may do, beyond normal user permissions). AppArmor is **path-based**, simpler to write/read (Debian default). SELinux is **label-based**, more granular and powerful but harder to configure (Rocky default).

**UFW vs firewalld** — both front-ends to the kernel firewall. UFW = simple allow/deny rules (Debian). firewalld = zone-based, dynamic rules without full reload (Rocky/RHEL).

**VirtualBox vs UTM** — VirtualBox: free, cross-platform type-2 hypervisor, great on x86 (Windows/Linux/Intel Mac). UTM: front-end to QEMU, the practical choice on Apple-Silicon Macs where VirtualBox doesn't run well. You used VirtualBox (Windows host).

**Why /boot unencrypted** — the bootloader must read the kernel before any passphrase is entered; an encrypted /boot couldn't be read at that stage.

**Interrupt the script without editing it** — it runs via cron, so stop the *scheduler entry* (comment the crontab line) or the cron service, not the file.

---

## Grade-0 traps to double-check
- GUI present · signatures differ · snapshot present at start · VM already running · root SSH allowed · any port besides 4242 open · README missing a required section/comparison · can't explain your own script.
