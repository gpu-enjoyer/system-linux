
# Backup Daemon

```
A system process that performs backups in the background.  
Linux shell CLI.
```

## Install

```bash
cd   "this project dir"
sudo ./install.sh
```

## Usage

|                             |                                                                            |
|-----------------------------|----------------------------------------------------------------------------|
| `backupd status`            | `systemctl status backupd.timer backupd.service`                           |
| `backupd enable`            | `sudo systemctl enable  --now backupd.timer`                               |
| `backupd disable`           | `sudo systemctl disable --now backupd.timer`                               |
| `backupd run`               | `sudo systemctl start backupd.service`                                     |
| `backupd log`               | `sudo journalctl -u backupd.timer -u backupd.service --since "1 hour ago"` |
| `backupd set source ...`    |  source directory                                                          |
| `backupd set target ...`    |  target directory                                                          |
| `backupd set frequency ...` |  frequency: 30s 10min 2h 3d 1w 1month 1y                                   |

## How does it work

```bash
./
├── code/
│   # src: backupd_run*
│
├── install.sh*
│   # ./rootfs/ -> /
│
└── rootfs/
    ├── etc/
    │   ├── default/
    │   │   └── backupd.conf
    │   │       # source,target
    │   └── systemd/
    │       └── system/
    │           ├── backupd.service
    │           │   # (get) env <- backupd.conf
    │           │   # (run) backupd_run*
    │           └── backupd.timer
    │               # (call) -> backupd.service
    └── usr/
        └── local/
            └── bin/
                ├── backupd*
                │   # CLI
                │   #  (backupd set source)    -> backupd.conf
                │   #  (backupd set target)    -> backupd.conf
                │   #  (backupd set frequency) -> backupd.timer
                └── backupd_run*
                    # do backup
```
