#!/bin/bash

DIR="$(dirname -- "${BASH_SOURCE[0]}";)";
DIR="$(realpath -e -- "$DIR";)";

# build kernel module
cd ${DIR}
echo "[run-release.sh] building n1.ko..."
cd ./n1.ko
bash ./build.sh

# build application
cd ${DIR}
echo "[run-release.sh] building n1..."
bash ./build-release.sh

# copy files
cd ${DIR}
echo "[run-release.sh] running..."
sudo ./build/release/n1 "./n1.ko/build/n1.ko"


