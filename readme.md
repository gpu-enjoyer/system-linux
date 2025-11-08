
# Backup Daemon

## Usage

```bash
# Edit ./etc/backupd/config.yaml
sudo make
```

## Roadmap

- [x] `class Daemon`
- [x] `Daemon.backup()`
- [x] prepare: `/etc/systemd/system/` + `/etc/backupd/`
- [ ] fill with content: `/etc/systemd/system/` + `/etc/backupd/`
- [ ] `backupd status`
- [ ] `backupd set-source`
- [ ] `backupd set-target`
- [ ] `backupd set-frequency`
- [ ] Protect OS system directories
