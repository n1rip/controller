#pragma once

#include "definitions.h"

void generate_mouse_input(int code, enum n1_mouse_action_e action, int value);
void generate_keyboard_input(int code, enum n1_keyboard_action_e action);
int download_file(const char* url, const char* path);
char* malloc_random_string(size_t len);
void init_rand_seed(void);
void setup_signal_callback(int signal, void (*handler)(int));
uintptr_t get_signature_offset(n1_process_t* process, n1_region_t* region, const char* signature);
uintptr_t get_offset(n1_process_t* proc, n1_region_t* region, n1_signature_t* sig);
int msleep(long msec);
int load_config(const char* str);