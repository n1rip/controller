#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/input-event-codes.h>
#include "../include/linux/module.h"
#include "../include/linux/ioctl.h"
#include "../include/globals.h"
#include "../include/definitions.h"
#include "../include/csgo/offset_manager.h"
#include "../include/csgo/definitions.h"
#include "../include/util.h"
#include "../include/csgo/hack.h"

int module_fd = 0;
int exit_signal = 0;

/**
 * @brief Handles interruption signal (ex: ctrl-c)
 * @param signal Signal number (in this case, SIGINT)
 */
void sigint_handler(int signal __attribute__((unused))) {
    exit_signal = 1;
}

/**
 * @brief Entry of the program
 */
int main(int argc, char** argv) {
    int status = 0;

    if (getuid() != 0) {
        DBG_PRINTF("error: program must be started as root\n");
        return 1;
    }

    if (argc != 2) {
        DBG_PRINTF("error: invalid syntax, usage: ./%s <path_to_kernel_module>\n", argv[0]);
        return 1;
    }

    setup_signal_callback(SIGINT, sigint_handler);

    if (load_module(argv[1]) != 0) {
        DBG_PRINTF("error: failed to load kernel module %s\n", argv[1]);
        return 1;
    }

    module_fd = open("/dev/n1", O_RDWR);
    if(module_fd < 0) {
        DBG_PRINTF("error: failed to link to kernel module\n");
        status = 1;
        goto open_fail;
    }

    status = csgo_run();

    close(module_fd);

open_fail:
    if (remove_module("n1") != 0) {
        DBG_PRINTF("error: failed to remove kernel module\n");
        status = 1;
    }

    return status;
}