#!/bin/bash

if [ -d "build/debug" ]; then
  rm -rf "build/debug"
fi

mkdir -p build/debug
make debug
cp ./n1 ./build/tmp
make clean
mv ./build/tmp ./build/debug/n1