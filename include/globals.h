#pragma once

extern int module_fd; /* holds the file descriptor to the kernel module */
extern int exit_signal; /* default 0; turns to 1 when an exit signal is detected (ex: ctrl-c) */