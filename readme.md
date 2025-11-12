
# Backup Daemon

A system process that performs backups in the background.  
Linux shell UI.


## Roadmap

- [x] `class Config`
- [x] `class Daemon`
- [x] `Daemon.backup()`
- [x] Install: `/etc/systemd/system/` + `/etc/backupd/` + `/usr/local/bin/`
- [x] Markup: `backupd.timer` + `backupd.service`
- [x] Protect OS system directories
- [x] Default права пользователя для `target_dir/`
- [x] Shell UI
- [ ] ? ~~`config.yaml`~~ стандартный config `/etc/default/backupd`
- [ ] Shell UI: set `source` `target` `frequency`


## Install

```bash
# edit: "./etc/backupd/config.yaml"
cd   "this project dir"
sudo ./install.sh
```

## Usage

|                   |                                                                            |
|-------------------|----------------------------------------------------------------------------|
| `backupd status`  | `systemctl status backupd.timer backupd.service`                           |
| `backupd enable`  | `sudo systemctl enable  --now backupd.timer`                               |
| `backupd disable` | `sudo systemctl disable --now backupd.timer`                               |
| `backupd run`     | `sudo systemctl start backupd.service`                                     |
| `backupd log`     | `sudo journalctl -u backupd.timer -u backupd.service --since "1 hour ago"` |




## Как это работает

1. `sudo install.sh` копирует файлы из `./rootfs/` в соответствующие папки `/`.  
    Также при установке в `/usr/local/bin/` отправляются:
    - `backupd` - скрипт UI с командами управления
    - `backupd_run` - исполняемый файл, скомпилированный из `./code/`

2. `backupd.timer` сообщает системе, что пора запускать  
    `backupd.service` по расписанию, указанному в `config.yaml`.
    
3. `backupd.service` в свою очередь отвечает за запуск `backupd_run`.
    
4. `backupd_run` при каждом запуске считывает `config.yaml`, и  
    если все в порядке, делает бэкап.

```
.
└─ rootfs
   ├── etc
   │   ├── backupd
   │   │   └── config.yaml
   │   └── systemd
   │       └── system
   │           ├── backupd.service
   │           └── backupd.timer
   └── usr
       └── local
           └── bin
               ├── backupd
               └── backupd_run
```
