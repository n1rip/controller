#!/bin/bash

DIR="$(dirname -- "${BASH_SOURCE[0]}";)";
DIR="$(realpath -e -- "$DIR";)";

# build kernel module
cd ${DIR}
echo "[run-release.sh] building kernel module..."
cd ./module
bash ./build.sh

# build application
cd ${DIR}
echo "[run-release.sh] building n1..."
bash ./build-release.sh

# copy files
cd ${DIR}
echo "[run-release.sh] running..."
sudo ./build/release/n1 "./module/build/n1.ko"


