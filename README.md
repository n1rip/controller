# n1

linux kernel-level external game hack using [n1.ko](https://github.com/eretsym/n1.ko). as this project is for educational purposes, every function has a comment describing their use. this solution has been architectured with modularity in mind: game-specific parts have their own directory and follow a specific flow that can be adapted to any other game.

## usage

1. clone solution

    ```sh
    git clone --recursive git@github.com:eretsym/n1.git && cd n1
    ```

2. install dependencies

    ```sh
    sudo zypper in libcurl-devel kernel-devel gcc
    ```

3. build and run

    ```sh
    ./run-debug.sh
    ```