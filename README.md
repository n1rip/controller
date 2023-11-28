# n1rip/controller

open-source kernel-level game hacking solution. this project has been made for educational purposes only. join the projects discord at https://discord.gg/PTYAeRdtHR

## usage

1. clone solution

    ```sh
    git clone --recursive git@github.com:n1rip/controller.git && cd controller
    ```

2. install dependencies

    ```sh
    sudo zypper in kernel-devel kernel-source gcc make 
    ```

3. update configuration in `./n1.example.cfg`

4. build and run

    ```sh
    ./run-debug.sh
    ```

## feedback

i'm open to feedback and improvements! please create [an issue](https://github.com/n1rip/controller/issues/new) for this purpose.