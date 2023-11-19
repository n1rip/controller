# n1rip/controller

open-source linux kernel-level external game hacking solution. as this project is for educational purposes, every function has a comment describing their use. this solution has been architectured with modularity in mind: game-specific parts have their own directory and follow a specific flow that can be adapted to any other game.

join our discord at https://discord.gg/PTYAeRdtHR

## usage

1. clone solution

    ```sh
    git clone --recursive git@github.com:n1rip/controller.git && cd controller
    ```

2. install dependencies

    ```sh
    sudo zypper in kernel-devel gcc make
    ```

3. build and run

    ```sh
    ./run-debug.sh
    ```