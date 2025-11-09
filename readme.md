
# Backup Daemon

## Usage

```bash
# Edit ./etc/backupd/config.yaml
# cd "project dir"
sudo make
```

## Как это работает

1. Установщик `sudo make` копирует файлы из локального `./etc` в системный `/etc`.  
    При установке бинарник `backupd` отправляется в `/usr/local/bin/`,  
    а конфиг `config.yaml` отправляется в `/etc/backupd/config.yaml`.  

2. *Описание работы сервиса и таймера.*  
    *Регулярный запуск бинарника c указаной в `config.yaml` частотой*.  

3. При каждом запуске бинарник считывает `/etc/backupd/config.yaml`  
    и если все в порядке, делает `backup()` по указанным в `config.yaml` директориям.

## Roadmap

- [x] `class Daemon`
- [x] `Daemon.backup()`
- [x] prepare: `/etc/systemd/system/` + `/etc/backupd/`
- [ ] markup: `/etc/systemd/system/` + `/etc/backupd/`
- [ ] `backupd status`
- [ ] `backupd set-source`
- [ ] `backupd set-target`
- [ ] `backupd set-frequency`
- [ ] Protect OS system directories
