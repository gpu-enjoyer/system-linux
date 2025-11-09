
all: check_yaml prepare_env build run

check_yaml:
	script/check_yaml.sh

prepare_env:
	script/prepare_env.sh

build:
	script/build.sh

run:
	/usr/local/bin/backupd
