# Born2beRoot — BONUS GUIDE (Debian 13, `anade-mo42`)

**How to use this:** same format as the build guide: `CONCEPT` (what it is and why) → `DO` (exact steps) → `CHECK` (the command that proves it; the evaluator will ask for these too).

The bonus has three parts:

1. **Partitions** laid out like the subject's example (7 logical volumes)
2. **WordPress** served by **lighttpd (HTTP server) + MariaDB (database) + PHP (dynamic page generation)**
3. **A service of your choice** ->  **Fail2ban**

---

## 3 RULES BEFORE YOU START

1. **The bonus is only graded if the mandatory part is PERFECT.** Don't break anything that already works: SSH on 4242, UFW, sudo rules, `monitoring.sh`.
2. **Back up first.** Shut the VM down and copy the whole `VirtualBox VMs/anade-mo42/` folder somewhere safe. Don't rely on snapshots: a snapshot must not exist at the defense, and while one exists the base `.vdi` stops changing, so its hash stops matching your real disk.
3. **Every boot changes the `.vdi` hash.** Do all the bonus work, then do the final shutdown → hash → `signature.txt` last (build guide, Phase 5).

---

# PART 1 — Bonus partitions

## Step 1.1 — What the subject wants

`CONCEPT` — The mandatory part only needs root, swap and home inside encrypted LVM. The bonus splits the system into more **logical volumes (LVs)**, each mounted on its own directory. Why a sysadmin does this: if one area fills up, it can't take the whole system down with it. For example, runaway logs fill `/var/log`, but `/` keeps working.

Target (from the subject; sizes are only an example, *you* choose and justify them):

```
sda                     disk
├─sda1        500M      part   /boot          ← plain, not encrypted
├─sda2          1K      part                  ← "extended" container (see below)
└─sda5                  part
  └─sda5_crypt          crypt                 ← LUKS encryption
    ├─LVMGroup-root      10G   lvm   /
    ├─LVMGroup-swap     2.3G   lvm   [SWAP]
    ├─LVMGroup-home       5G   lvm   /home
    ├─LVMGroup-var        3G   lvm   /var
    ├─LVMGroup-srv        3G   lvm   /srv
    ├─LVMGroup-tmp        3G   lvm   /tmp
    └─LVMGroup-var--log   4G   lvm   /var/log
```

What each piece is:

| Mount | Holds | Why separate |
|---|---|---|
| `/` | the OS itself | must never fill up |
| `swap` | RAM overflow | ≈ RAM size (your VM has ~2 GB) |
| `/home` | users' files | users can't fill the system disk |
| `/var` | variable data: **WordPress** (`/var/www`) and **MariaDB** (`/var/lib/mysql`) | the web site grows here, so give it room |
| `/srv` | data served by the machine (FTP, sites…) | kept apart from the system |
| `/tmp` | temporary files | anyone can write here, so it's contained |
| `/var/log` | logs (sudo logs included) | logs can explode; they must not fill `/var` |

**Two things evaluators ask about:**
- **Why `sda2` is 1K and the data partition is `sda5`:** an old-style (MBR) disk allows only 4 *primary* partitions. To get more, one of them becomes an **extended** partition, a container holding **logical** partitions. The extended container shows as 1K, and logical partitions are always numbered from 5. You get this layout by creating the big partition as **Logical**.
- **Why the name is `var--log`:** the device name is `VGname-LVname`. A dash inside a name gets doubled so it can't be confused with the separator. The LV is really called `var-log`.

## Step 1.2 — Is this already done on your VM?

`CHECK` — On the VM:
```bash
lsblk
```
- **You see all 7 LVs** (root, swap, home, var, srv, tmp, var--log) → Part 1 is done. Skip to Step 1.4 to verify the mounts.
- **You see only root, swap, home** → the clean way is a reinstall using Step 1.3. Moving `/var` onto a new volume on a running system is possible but delicate; ask before trying it.

## Step 1.3 — The installer steps (reference)

`DO` — At the Debian installer's **Partition disks** screen:

1. Method: **Manual** → select the disk → **Yes** (new empty partition table).
2. **FREE SPACE** → Create new partition → **500 MB** → **Primary** → **Beginning** → *Use as* **Ext4** · *Mount point* **/boot** → Done.
3. **FREE SPACE** → Create new partition → **max** (all the rest) → **Logical** → *Use as* **physical volume for encryption** → Done. *(Logical is what gives you `sda2` + `sda5`.)*
4. **Configure encrypted volumes** → Yes → Create → select `sda5` → Finish → Yes (erase data, slow, let it run) → set the **passphrase** twice.
5. **Configure the Logical Volume Manager** → Yes → **Create volume group** → name **`LVMGroup`** → select `/dev/mapper/sda5_crypt`.
6. **Create logical volume** 7 times, all in `LVMGroup`:

   | LV name | Size |
   |---|---|
   | `root` | 10G |
   | `swap` | 2.3G |
   | `home` | 5G |
   | `var` | 3G |
   | `srv` | 3G |
   | `tmp` | 3G |
   | `var-log` | 4G (or whatever space is left) |

   → **Finish**.
7. Back in the list, set each LV (select the line under each LV, `#1 ...`):

   | LV | Use as | Mount point |
   |---|---|---|
   | root | Ext4 | `/` |
   | swap | swap area | – |
   | home | Ext4 | `/home` |
   | var | Ext4 | `/var` |
   | srv | Ext4 | `/srv` |
   | tmp | Ext4 | `/tmp` |
   | var-log | Ext4 | **Enter manually** → `/var/log` |

   `/var/log` isn't in the installer's preset list, which is why you type it.
8. **Finish partitioning and write changes to disk** → Yes.

*Sizes won't be round* (499.1M etc.): the installer counts in MB (1000²) while `lsblk` counts in MiB (1024²), and partitions are aligned to 1 MiB. That's normal.

## Step 1.4 — Verify

`CHECK` —
```bash
lsblk                          # same tree as the subject
sudo vgs                       # one VG: LVMGroup
sudo lvs                       # 7 LVs
findmnt /tmp                   # SOURCE must be /dev/mapper/LVMGroup-tmp
df -h | grep mapper            # each LV mounted where it should be
```

**Debian 13 gotcha:** on Debian 13, `/tmp` is a RAM-backed `tmpfs` by default. An `/etc/fstab` entry overrides that, so with the layout above `findmnt /tmp` should show your LV. If it shows `tmpfs`, check that `/etc/fstab` has a `/tmp` line.

**Defense lines to be ready for:**
- "Why these sizes?" → `/var` and `/var/log` get room because the web site, the database and the logs live there; `/tmp` and `/srv` stay small because nothing big lives there; swap ≈ RAM.
- "Why `/boot` outside?" → same answer as the mandatory part: the bootloader reads it before the passphrase is typed.

---

# PART 2 — WordPress (lighttpd + MariaDB + PHP)

## Step 2.1 — The big picture

`CONCEPT` — A WordPress site is four pieces working together:

```
browser ──HTTP──▶ lighttpd ──FastCGI──▶ PHP ──SQL──▶ MariaDB
(Windows)         web server           runs WordPress   stores posts, users
```

- **lighttpd** ("lighty"): a small, fast **web server**. It receives the browser's request and sends back files. It can't run PHP code by itself.
- **PHP**: the language WordPress is written in. **php-cgi** runs PHP code for lighttpd.
- **FastCGI**: the protocol lighttpd uses to hand `.php` files to PHP and get the HTML back.
- **MariaDB**: a **database server** (a MySQL fork). WordPress stores everything there: posts, users, settings.
- **WordPress**: a PHP application, i.e. a folder of `.php` files placed where lighttpd serves files from (`/var/www/html`).

## Step 2.2 — Open the way in (port 80)

`CONCEPT` — Web servers listen on **port 80** (HTTP). The subject allows extra ports for the bonus, as long as UFW matches. The VM sits behind VirtualBox NAT, so Windows also needs a forwarding rule, just like for SSH.

`DO` —
1. VirtualBox (VM can stay on) → **Settings → Network → Adapter 1 → Advanced → Port Forwarding** → add:
   `Name: http · Protocol: TCP · Host Port: 8080 · Guest Port: 80`
   (8080 on Windows because port 80 is often taken or needs admin rights.)
2. On the VM:
   ```bash
   sudo ufw allow 80
   sudo ufw status numbered       # 4242 and 80
   ```

## Step 2.3 — lighttpd

`DO` —
```bash
sudo apt update
sudo apt install -y lighttpd
sudo systemctl enable --now lighttpd
```

`CHECK` — In the Windows browser: **http://localhost:8080** → lighttpd's placeholder page.
```bash
sudo systemctl status lighttpd     # active (running)
sudo ss -tlnp | grep :80           # lighttpd listening
```

## Step 2.4 — PHP, connected to lighttpd

`CONCEPT` — **Trap:** `sudo apt install php` pulls in **Apache2** as a dependency, and Apache2 is forbidden by the subject. Install `php-cgi` (the piece lighttpd uses) plus the extensions WordPress needs, by name.

`DO` —
```bash
sudo apt install -y php-cgi php-mysql php-curl php-gd php-mbstring php-xml php-zip
sudo lighty-enable-mod fastcgi fastcgi-php
sudo systemctl restart lighttpd
```
- `php-mysql` → lets PHP talk to MariaDB.
- `lighty-enable-mod` → switches on lighttpd's FastCGI module and its PHP config (it links files into `/etc/lighttpd/conf-enabled/`).

`CHECK` —
```bash
dpkg -l | grep -i apache2          # must print NOTHING
ls /etc/lighttpd/conf-enabled/     # 10-fastcgi.conf, 15-fastcgi-php.conf
```
Quick PHP test, then delete it (it exposes server details):
```bash
echo '<?php phpinfo();' | sudo tee /var/www/html/info.php
# browser: http://localhost:8080/info.php → purple PHP page
sudo rm /var/www/html/info.php
```

## Step 2.5 — MariaDB: database + user for WordPress

`CONCEPT` — WordPress needs a **database** (a named container of tables) and a **database user** allowed to use it, and only it. Database users are separate from Linux users. `'wpuser'@'localhost'` means this account can only connect from the VM itself, never from the network.

`DO` —
```bash
sudo apt install -y mariadb-server
sudo systemctl enable --now mariadb
sudo mariadb-secure-installation
```
Answers for `mariadb-secure-installation`: current root password → just Enter · switch to unix_socket → **n** · change root password → **n** (root logs in through `sudo`) · remove anonymous users → **Y** · disallow root login remotely → **Y** · remove test database → **Y** · reload privileges → **Y**.

Create the database (pick your own password and write it in your private secrets file):
```bash
sudo mariadb
```
```sql
CREATE DATABASE wordpress;
CREATE USER 'wpuser'@'localhost' IDENTIFIED BY 'YOUR_DB_PASSWORD';
GRANT ALL PRIVILEGES ON wordpress.* TO 'wpuser'@'localhost';
FLUSH PRIVILEGES;
EXIT;
```

`CHECK` —
```bash
mariadb -u wpuser -p -e "SHOW DATABASES;"    # asks the password, lists: wordpress
```

## Step 2.6 — WordPress files

`DO` —
```bash
cd /tmp
wget https://wordpress.org/latest.tar.gz     # if wget is missing: sudo apt install -y wget
tar -xzf latest.tar.gz                       # creates /tmp/wordpress
sudo rm /var/www/html/index.lighttpd.html    # lighttpd's placeholder page
sudo cp -r wordpress/. /var/www/html/
sudo chown -R www-data:www-data /var/www/html
```
- `www-data` is the Linux user lighttpd and PHP run as. WordPress must own its files to install plugins and upload images.

Connect WordPress to the database:
```bash
cd /var/www/html
sudo -u www-data cp wp-config-sample.php wp-config.php
sudo vim wp-config.php
```
Change these three lines:
```php
define( 'DB_NAME', 'wordpress' );
define( 'DB_USER', 'wpuser' );
define( 'DB_PASSWORD', 'YOUR_DB_PASSWORD' );
```
In the same file, replace the block of `put your unique phrase here` lines with fresh keys. Open **https://api.wordpress.org/secret-key/1.1/salt/** in the Windows browser and paste its output over that block. The keys sign login cookies.

## Step 2.7 — Install from the browser

`DO` — Windows browser → **http://localhost:8080** → WordPress setup: language → site title, admin username, admin password (not your Linux one; save it in your secrets file), email → **Install WordPress** → log in.

`CHECK` —
- **http://localhost:8080** shows the site.
- **http://localhost:8080/wp-admin** → dashboard. Write a test post so the evaluator sees it works.
- Data really is in MariaDB:
  ```bash
  sudo mariadb -e "USE wordpress; SHOW TABLES;"   # wp_posts, wp_users, ...
  ```

## Step 2.8 — Optional: import your films with WP-CLI

`CONCEPT` — **WP-CLI** (`wp`) manages WordPress from the terminal: create posts, categories and media without the browser. The import turns each film of your website's `films.json` into a post: its description, a details list, the IMDb link, its picture as featured image, and its type as category. Two scripts, in `~/42/cursus/born2beroot/bonus/wp_films/`:

| Script | Runs on | Does |
|---|---|---|
| `prepare_films.py` | WSL | copies `films.json` from `D:\website`, converts the `.avif` pictures to `.jpg`, fills `package/` |
| `import_films.sh` | VM | checks → categories → one post per film → featured images; skips films already imported |

`DO` — On WSL (already done once; rerun only if the website changes):
```bash
cd ~/42/cursus/born2beroot/bonus/wp_films
python3 prepare_films.py
```

On the VM, install the tools (`php-cli` runs PHP from the terminal; it does **not** pull Apache):
```bash
sudo apt install -y php-cli jq curl
curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
php wp-cli.phar --info                 # prints PHP and WP-CLI versions
chmod +x wp-cli.phar
sudo mv wp-cli.phar /usr/local/bin/wp
dpkg -l | grep -i apache2              # still nothing
```

Copy the package from Windows PowerShell:
```powershell
scp -P 4242 -r \\wsl$\Ubuntu\home\anachiossi\42\cursus\born2beroot\bonus\wp_films\package anade-mo@localhost:~/wp_films
```

Run it on the VM:
```bash
cd ~/wp_films
sudo bash import_films.sh
```
It prints one line per post and ends with `done: N new posts`. Running it again only adds films that are missing.

`CHECK` —
```bash
sudo -u www-data wp --path=/var/www/html post list --meta_key=film_id --format=count   # 65
```
Browser → `http://localhost:8080`: posts with pictures, newest shoot first; categories Film, TV Series…

**Undo** (deletes the imported posts and their pictures, nothing else):
```bash
cd /var/www/html
sudo -u www-data wp post delete $(sudo -u www-data wp post list --post_type=post --post_status=any --meta_key=film_id --format=ids) --force
sudo -u www-data wp post delete $(sudo -u www-data wp post list --post_type=attachment --post_status=any --meta_key=film_id --format=ids) --force
```

**Defense line:** "WP-CLI talks to WordPress's own PHP code, so the posts go into MariaDB exactly as if I had typed them in the admin page."

**Defense lines to be ready for:**
- "What does each service do?" → use the diagram in 2.1.
- "Why lighttpd?" → the subject requires it. It's lightweight: little RAM, good for a small VM.
- "Why port 80 and 8080?" → 80 is HTTP inside the VM (UFW allows it); VirtualBox forwards Windows 8080 to it.
- "Is Apache installed?" → `dpkg -l | grep apache2` → nothing.

---

# PART 3 — Service of your choice: Fail2ban

## Step 3.1 — Why this one (your defense justification)

`CONCEPT` — **Fail2ban** watches login logs. When one IP address fails too many times in a short window, it **bans** that IP by adding a firewall rule for a set time.

Why it's useful *on this server*:
- **SSH is exposed on 4242.** Moving the port only cuts noise; a bot that finds it can still guess passwords forever. Fail2ban stops that after a few attempts.
- **It complements what you already have.** The password policy makes passwords hard to guess, UFW decides *which ports* are open, and Fail2ban decides *who* gets blocked on an open port.
- **It works with UFW:** bans appear as UFW rules you can show live.
- **It's light:** no web interface, no extra open ports.

## Step 3.2 — Install and configure

`CONCEPT` — On Debian 13, logins are recorded in the **systemd journal**, not in `/var/log/auth.log` (that file doesn't exist by default). So Fail2ban must read the journal: `backend = systemd`. Settings go in **`jail.local`**, never in `jail.conf`, which package updates overwrite. A **jail** = one service being watched plus its rules.

`DO` —
```bash
sudo apt install -y fail2ban python3-systemd
sudo vim /etc/fail2ban/jail.local
```
Write:
```ini
[DEFAULT]
banaction = ufw

[sshd]
enabled  = true
port     = 4242
backend  = systemd
maxretry = 5
findtime = 10m
bantime  = 2m
```
- `banaction = ufw` → bans are added as UFW deny rules.
- `port = 4242` → your SSH port, not the default 22.
- `maxretry` / `findtime` → 5 failures within 10 minutes …
- `bantime` → … gives a 2-minute ban (short on purpose, so the ban can be
  demonstrated at the defense without locking anyone out for long).

```bash
sudo systemctl enable fail2ban
sudo systemctl restart fail2ban
```

`CHECK` —
```bash
sudo systemctl status fail2ban         # active (running)
sudo fail2ban-client status            # Jail list: sshd
sudo fail2ban-client status sshd       # failed / banned counters
```

## Step 3.3 — Live test (careful)

`CONCEPT` — Because of VirtualBox NAT, **every connection from Windows reaches the VM from the same address, `10.0.2.2`.** If you ban it, *all* your SSH sessions from Windows/WSL are blocked, including the evaluator's. Existing sessions stay open, so keep one open (or use the VM window) to unban.

`DO` —
1. Keep one SSH session open (or be logged in on the VM window).
2. From a new Windows/WSL terminal, connect with a wrong password 3 times:
   ```bash
   ssh anade-mo@localhost -p 4242
   ```
3. On the VM:
   ```bash
   sudo fail2ban-client status sshd      # Banned IP list: 10.0.2.2
   sudo ufw status numbered              # a DENY rule for 10.0.2.2 at the top
   ```
4. Unban right away:
   ```bash
   sudo fail2ban-client set sshd unbanip 10.0.2.2
   sudo ufw status numbered              # rule gone
   ```

**Before the defense:** make sure no ban is active (`sudo fail2ban-client status sshd`). An extra rule in `ufw status` would confuse the "only these ports" check.

---

# BONUS DEFENSE CHEAT SHEET

| They ask | You run | You say |
|---|---|---|
| Partitions | `lsblk` | 7 LVs in encrypted `LVMGroup`; why `sda2`=1K / `sda5`; size choices |
| Ports | `sudo ufw status numbered` | 4242 = SSH, 80 = WordPress; subject allows extra ports for bonus |
| Web server | `systemctl status lighttpd` | serves files, hands `.php` to PHP via FastCGI |
| No Apache/NGINX | `dpkg -l \| grep -E "apache2\|nginx"` | nothing installed |
| Database | `sudo mariadb -e "SHOW DATABASES;"` | `wordpress` DB, dedicated `wpuser` limited to localhost |
| Site works | browser → `http://localhost:8080` | NAT forwards 8080 → VM 80 |
| Your service | `sudo fail2ban-client status sshd` | bans brute-force IPs on SSH via UFW; justification in 3.1 |

---

## End-of-bonus checklist
- [ ] `lsblk` shows the 7 LVs; `findmnt /tmp` shows the LV, not tmpfs
- [ ] lighttpd, mariadb, fail2ban all `active (running)` and `enabled`
- [ ] `dpkg -l | grep -E "apache2|nginx"` prints nothing
- [ ] UFW: 4242 and 80 only; no active Fail2ban ban
- [ ] WordPress site opens at `http://localhost:8080`, admin login works, a test post exists
- [ ] `info.php` deleted
- [ ] DB password and WordPress admin password saved in your private secrets file (never in git)
- [ ] Mandatory part still perfect (re-run the build guide checklist)
- [ ] README mentions the bonus design choices (partition sizes, WordPress stack, why Fail2ban)
- [ ] Final shutdown → new `.vdi` hash → `signature.txt` updated
