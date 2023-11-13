# n1

open-source linux kernel-level external game hack. as this project is for educational purposes, every function has a comment describing their use. this solution has been architectured with modularity in mind: game-specific parts have their own directory and follow a specific flow that can be adapted to any other game.

## usage

1. clone solution

    ```sh
    git clone --recursive git@github.com:n1rip/controller.git && cd controller
    ```

2. install dependencies

    ```sh
    sudo zypper in libcurl-devel kernel-devel gcc
    ```

3. build and run

    ```sh
    ./run-debug.sh
    ```