#!/bin/bash

DIR="$(dirname -- "${BASH_SOURCE[0]}";)";
DIR="$(realpath -e -- "$DIR";)";

# build kernel module
cd ${DIR}
echo "[run-debug.sh] building kernel module..."
cd ./module
bash ./build.sh

# build application
cd ${DIR}
echo "[run-debug.sh] building n1..."
bash ./build-debug.sh

# copy files
cd ${DIR}
echo "[run-debug.sh] running..."
sudo ./build/debug/n1 "./module/build/n1.ko"


