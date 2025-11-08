
# Backup Daemon

```bash

## Usage

```bash
# Edit config.yaml

# Optional: create system log symlink
ln -s /var/log/syslog syslog-link

# Run
make
```

```
 Todo:

   sudo systemctl start <service>
   sudo systemctl stop <service>
   sudo systemctl restart <service>
   sudo systemctl reload <service>

   sudo systemctl enable <service>
   sudo systemctl disable <service>
   sudo systemctl is-enabled <service>

```


## Roadmap

- [x] read(`config.yaml`)
- [ ] `backup(fs::path source, target)`
- [ ] `sleep(freq)`
- [ ] when the OS shutdown
- [ ] when the OS starts
- [ ] shell UI
- [x] protect system files


## Todo

### main features

```
thread:
    wake up
    do backup
    sleep
```

```
when the OS off ...
```

```
when the OS on ...
```

### config.yaml

```
Где хранить конфиг yaml?
Как правильно организовать с ним взаимодействие?
```


### shell UI

```bash
backupd --command
#
#   <->
#
sudo systemctl "command" "backupd"
```

```bash
backupd --help
# manual ...
```

```bash
backupd --run
# ...
```

```bash
backupd --stop
# ...
```

```bash
backupd --status
# path to source dir: ...
# path to target dir: ...
# frequency:          ...
```


## Useful info

### systemd

```bash
man systemd

#   systemd is a system and service manager for Linux
#   operating systems. When run as first process on
#   boot (as PID 1), it acts as init system that brings
#   up and maintains userspace services. Separate
#   instances are started for logged-in users to start
#   their services.

#   ...

#   systemd provides a dependency system between
#   various entities called "units" of 11 different
#   types.
#   Units encapsulate various objects that are
#   relevant for system boot-up and maintenance.
#   The majority of units are configured in unit
#   configuration files, whose syntax and basic set of
#   options is described in 'systemd.unit'.
#   However, some are created automatically from other
#   configuration files, dynamically from system state
#   or programmatically at runtime.

#   ...

#   The following unit types are available:
#
#   1. Service units, which start and control daemons
#   and the processes they consist of.
#   For details, see 'systemd.service'.
#
#   ...
```

### systemd.unit

```bash
man systemd.unit

#   Возможно, тоже стоит почитать
```

### systemctl

```bash
man systemctl

#   systemctl may be used to introspect and control the
#   state of the "systemd" system and service manager.
```

### systemctl features

```bash
systemctl list-timers

#   Судя по всему, можно зарегистрировать
#   системный таймер для демона
```

```bash
systemctl list-units

#   Просто интересно посмотреть
```

```bash
systemctl status [PATTERN...|PID...]]

#   Show runtime status information about the whole
#   system or about one or more units followed by
#   most recent log data from the journal.
```


## Notes

Все системные службы пишут в `.../syslog`  

Уровни важности:  
`LOG_DEBUG`, `LOG_INFO`, `LOG_NOTICE`, `LOG_WARNING`, `LOG_ERR`, `LOG_CRIT`  

Маршрутизация:  
Можно настроить `rsyslog/syslog-ng` для отправки логов в файлы, на удалённый сервер или в базу данных.

Асинхронность:  
Демон пишет в `syslog` без необходимости напрямую открывать файлы - ядро берёт на себя запись.  
Не нужно заботиться о блокировках файлов.  

Инструмены мониторинга:  
`logwatch`, `journalctl`, `systemd`