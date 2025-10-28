
# Backup Daemon

```bash
#
# Todo:
#
#   sudo systemctl start <service>
#   sudo systemctl stop <service>
#   sudo systemctl restart <service>
#   sudo systemctl reload <service>
#
#   sudo systemctl enable <service>
#   sudo systemctl disable <service>
#   sudo systemctl is-enabled <service>
#

# Edit config.yaml

# Create system log symlink
ln -s /var/log/syslog syslog-link

# Run
make

# Check ./syslog-link

# Todo: shell UI
backup-daemon --help
backup-daemon --restart
backup-daemon --stop
```


## Roadmap

- [x] read(config.yaml)
- [ ] backup(fs::path source, target)
- [ ] sleep(freq)
- [ ] save(Time) before OS shutdown
- [ ] wake_up(when OS_starts)
- [ ] shell UI
- [ ] protect system files


## syslog

Стандартная система логирования `Unix`.  
Имеет ряд преимуществ для демонов и системных сервисов:

1. Централизованное логирование  
Все системные службы пишут в единое место: `/var/log/syslog` и др.  

2. Разделение по уровням  
Есть уровни важности: `LOG_DEBUG`, `LOG_INFO`, `LOG_NOTICE`, `LOG_WARNING`, `LOG_ERR`, `LOG_CRIT` и др.  
Позволяет фильтровать сообщения по критичности.  

3. Удобная маршрутизация  
Можно настроить `rsyslog/syslog-ng` для отправки логов в разные файлы, на удалённый сервер или в базу данных.

4. Асинхронность  
Демон пишет в `syslog` без необходимости напрямую открывать файлы — ядро берёт на себя запись.
Не нужно заботиться о блокировках файлов.

5. Стандарт для демонов  
Большинство системных сервисов используют именно `syslog`.
Совместимость с инструментами мониторинга (`logwatch`, `journalctl`, `systemd`).
