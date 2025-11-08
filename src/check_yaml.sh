#!/bin/bash

HAVE_YAML=$(
    echo "#include <yaml-cpp/yaml.h>" | \
    g++ -x c++ - -c -o /dev/null >/dev/null 2>/dev/null \
    && echo yes || echo no
)

if [ "$HAVE_YAML" = "no" ]; then
    echo -e "Please install: \n  sudo apt install libyaml-cpp-dev"
    exit 1
fi
