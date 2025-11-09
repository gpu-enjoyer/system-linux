#!/bin/bash

set -e

BIN=/usr/local/bin/backupd

rm -f "$BIN"
g++ -std=c++17 code/main.cpp code/backupd.cpp -lyaml-cpp -o "$BIN"
