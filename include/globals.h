#pragma once

#include "./definitions.h"

extern int module_fd; /* holds the file descriptor to the kernel module */
extern int exit_signal; /* default 0; turns to 1 when an exit signal is detected (ex: ctrl-c) */
extern n1_config_t config; /* holds the config information */