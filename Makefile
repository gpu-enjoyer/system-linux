
HAVE_YAML := $(shell test -f /usr/include/yaml-cpp/yaml.h && echo yes || echo no)

ifeq ($(HAVE_YAML),no)
$(error Please install 'sudo apt install libyaml-cpp-dev')
endif

all: build run

build:
	rm -rf .build
	mkdir .build
	g++ -std=c++17 daemon.cpp -lyaml-cpp -o .build/daemon

run:
	.build/daemon
