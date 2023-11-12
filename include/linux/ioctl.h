#pragma once

#include <stdint.h>
#include "../definitions.h"

int get_process(const char* name, n1_process_t *proc);
int get_region(n1_process_t *proc, const char* name, n1_region_t *region);
long read_memory(n1_process_t *proc, uintptr_t address, size_t len, void *buffer);
long write_memory(n1_process_t *proc, uintptr_t address, size_t len, void *buffer);
int generate_input(int code, int value, int type, enum n1_input_device_type_e device_type);
enum n1_key_state_e get_key_state(int code);