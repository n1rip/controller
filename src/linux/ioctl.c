#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/linux/ioctl.h"
#include "../../include/globals.h"

#define N1_GET_PROCESS _IOWR(0x22, 0, struct n1_process_s *)
#define N1_GET_REGION _IOWR(0x22, 1, struct n1_region_s *)
#define N1_READ _IOWR(0x22, 2, struct n1_rw_s *)
#define N1_WRITE _IOWR(0x22, 3, struct n1_rw_s *)
#define N1_GENERATE_INPUT _IOWR(0x22, 4, struct n1_input_s *)
#define N1_GET_KEY_STATE _IOWR(0x22, 5, struct n1_key_state_s *)

/**
 * @brief Gets a process using its name
 * @param name Name of the process to find (In)
 * @param proc process to get
 * @return Pointer to a new process struct, NULL on error
 * @note Don't forget to free
 */
int get_process(const char* name, n1_process_t* proc) {
    proc->pid = 0;
    strcpy(proc->name, name);
    
    if (ioctl(module_fd, N1_GET_PROCESS, proc) != 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief Gets a certain memory region from a process
 * @param proc Process structure containing the info about the target process (In)
 * @param name Name of the target memory region (In)
 * @param region Pointer to an allocated n1_region_t struct (Out)
 * @return 0 on success, 1 on error
 */
int get_region(n1_process_t *proc, const char* name, n1_region_t *region) {
    region->pid = proc->pid;
    strcpy(region->name, name);

    if (ioctl(module_fd, N1_GET_REGION, region) != 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief Reads a certain length of a process memory at a certain address
 * @param proc Process structure that we read from (In)
 * @param address Address of the data in the process (In)
 * @param len Length of the data to read (In)
 * @param buffer Gets filled by the read request (Out)
 * @return The total length minus the number of bytes read. -1 on error
 */
ssize_t read_memory(n1_process_t *proc, uintptr_t address, size_t len, void *buffer) {
    n1_rw_t request;

    request.pid = proc->pid;
    request.address = address;
    request.len = len;
    request.buffer_address = (uintptr_t)buffer;

    if (ioctl(module_fd, N1_READ, &request) != 0) {
        return -1;
    }

    return (ssize_t)request.len;
}

/**
 * @brief Writes a certain buffer in a process memory at a certain address
 * @param proc Process structure that we write to (In)
 * @param address Address of the data in the process (In)
 * @param len Length of the data to write (In)
 * @param buffer Buffer content to write to the process (In)
 * @return The total length minus the number of bytes written. -1 on error
 */
ssize_t write_memory(n1_process_t *proc, uintptr_t address, size_t len, void *buffer) {
    n1_rw_t request;

    request.pid = proc->pid;
    request.address = address;
    request.len = len;
    request.buffer_address = (uintptr_t)buffer;

    if (ioctl(module_fd, N1_WRITE, &request) != 0) {
        return -1;
    }

    return (ssize_t)request.len;
}

/**
 * @brief Generates a keyboard input
 * @param code Key code (see: input-event-codes.h)
 * @param value Movement value or press/release value
 * @param type Event type (EV_KEY, EV_REL...)
 * @return 0 on success, 1 otherwise
 */
int generate_input(int code, int value, int type, enum n1_input_device_type_e device_type) {
    n1_input_t request;

    request.code = code;
    request.value = value;
    request.type = type;
    request.device_type = device_type;

    if (ioctl(module_fd, N1_GENERATE_INPUT, &request) != 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief Gets a key state (pressed/released)
 * @param code Key code (see: input-event-codes.h)
 * @return n1_key_state_e
 */
enum n1_key_state_e get_key_state(int code) {
    n1_key_state_t request;

    request.code = code;
    request.state = key_state_released;

    ioctl(module_fd, N1_GET_KEY_STATE, &request);

    return request.state;
}