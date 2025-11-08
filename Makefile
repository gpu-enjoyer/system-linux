
all: check_yaml build run

check_yaml:
	src/check_yaml.sh

build:
	rm -rf .build
	mkdir .build
	g++ -std=c++17 src/main.cpp src/daemon.cpp -lyaml-cpp -o .build/daemon

run:
	.build/daemon
