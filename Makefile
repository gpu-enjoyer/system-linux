
all: check_yaml prepare_env build run

check_yaml:
	script/check_yaml.sh

prepare_env:
	script/prepare_env.sh

build:
	rm -rf .build
	mkdir .build
	g++ -std=c++17 code/main.cpp code/daemon.cpp -lyaml-cpp -o .build/daemon

run:
	.build/daemon
