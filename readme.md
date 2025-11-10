
# Backup Daemon


## Roadmap

- [x] `class Config`
- [x] `class Daemon`
- [x] `Daemon.backup()`
- [x] Install: `/etc/systemd/system/` + `/etc/backupd/` + `/usr/local/bin/`
- [x] Markup: `backupd.timer` + `backupd.service`
- [ ] ? ~~`config.yaml`~~ стандартный config `/etc/default/backupd`
- [ ] UI: `backupd status`
- [ ] UI: `backupd` + set `source target frequency`
- [ ] Protect OS system directories
- [ ] Default права пользователя для `target_dir/`


## Usage

```bash
# Edit ./etc/backupd/config.yaml
# cd "project dir"
sudo make
sudo systemctl daemon-reload
sudo systemctl enable --now backupd.timer
# ...
sudo systemctl disable --now backupd.timer
```


## Useful for debug

```bash
sudo journalctl -u backupd.timer -u backupd.service

systemctl status backupd.service
systemctl status backupd.timer

sudo systemctl start backupd.service
```


## Как это работает

1. `sudo make` копирует файлы из `./etc` в системный `/etc`.  
    Также при установке исполняемый файл `backupd`  
    отправляется в `/usr/local/bin/`.

1. `backupd.timer` сообщает системе, что пора запускать  
    `backupd.service` по расписанию, указанному в `config.yaml`.
    
2. `backupd.service` в свою очередь отвечает за запуск  
   исполняемого файла `backupd`.
    
3. `backupd` при каждом запуске считывает `config.yaml`, и  
    если все в порядке, делает `backup()`.

```
.
├── Makefile
├── etc
    ├── backupd
    │   └── config.yaml
    └── systemd
        └── system
            ├── backupd.service
            └── backupd.timer
```
