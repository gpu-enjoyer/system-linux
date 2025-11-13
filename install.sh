#!/bin/bash

set -e

CONF=etc/default
UNIT=etc/systemd/system
BINR=usr/local/bin
EXE=./rootfs/$BINR/backupd_run


# Prepare environment

rm -rf /$CONF/backupd*
rm -rf /$UNIT/backupd*
rm -ff /$BINR/backupd*

mkdir -p  /$CONF /$UNIT /$BINR

rm -f $EXE
g++ -std=c++17 code/backupd_run.cpp -o $EXE
chmod 711 $EXE # 421--1--1

cp -r ./rootfs/$CONF/* /$CONF/
cp -r ./rootfs/$UNIT/* /$UNIT/
cp -r ./rootfs/$BINR/* /$BINR/

ln -s /var/log/syslog syslog-link || true


# Run daemon live-cycle

systemctl daemon-reload
systemctl enable --now backupd.timer

systemctl status backupd.timer || true
echo "backupd installed"
