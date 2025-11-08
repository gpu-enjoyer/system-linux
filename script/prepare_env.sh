#!/bin/bash

mkdir -p /etc/backupd/

cp -r  etc/systemd/system/*  /etc/systemd/system/
cp -r  etc/backupd/*         /etc/backupd/
