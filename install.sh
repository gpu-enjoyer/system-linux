#!/bin/bash

set -e

HAVE_YAML=$(
    echo "#include <yaml-cpp/yaml.h>" | \
    g++ -x c++ - -c -o /dev/null >/dev/null 2>/dev/null \
    && echo yes || echo no
)
if [ "$HAVE_YAML" = "no" ]; then
    echo -e "Please install: \n  sudo apt install libyaml-cpp-dev"
    exit 1
fi


CONF=etc/backupd
UNIT=etc/systemd/system
BINR=usr/local/bin

EXE=./rootfs/$BINR/backupd_run
rm -f "$EXE"
g++ -std=c++17 code/main.cpp code/backupd.cpp -lyaml-cpp -o $EXE
chmod 711 $EXE

rm -rf /$CONF
rm -rf /$UNIT/backupd.*
rm -rf /$BINR/backupd*

mkdir -p /$CONF
mkdir -p /$UNIT
mkdir -p /$BINR

cp -r ./rootfs/$CONF/* /$CONF/
cp -r ./rootfs/$UNIT/* /$UNIT/
cp -r ./rootfs/$BINR/* /$BINR/


ln -s /var/log/syslog syslog-link || true

systemctl daemon-reload
systemctl enable --now backupd.timer

systemctl start  backupd.service

systemctl status backupd.timer   || true
systemctl status backupd.service || true

echo "backupd installed"
