
# Backup Daemon


## Roadmap

- [x] `class Config`
- [x] `class Daemon`
- [x] `Daemon.backup()`
- [x] prepare: `/etc/systemd/system/` + `/etc/backupd/` + `/usr/local/bin/`
- [ ] markup: `backupd.timer` + `backupd.service`
- [ ] UI: `backupd status`
- [ ] UI: `backupd` + `source=` / `target=` / `frequency=`
- [ ] Protect OS system directories


## Usage

```bash
# Edit ./etc/backupd/config.yaml
# cd "project dir"
sudo make
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
