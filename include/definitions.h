#pragma once

#include <sys/types.h>
#include <stdint.h>

#ifdef DEBUG
#define DBG_PRINTF(...) printf(__VA_ARGS__);
#else
#define DBG_PRINTF(...)
#endif

/**
 * @brief Types of keyboard actions
 */
enum n1_keyboard_action_e {
    keyboard_action_press,
    keyboard_action_release,
    keyboard_action_actuate
};

/**
 * @brief Types of mouse actions
 */
enum n1_mouse_action_e {
    mouse_action_press,
    mouse_action_release,
    mouse_action_actuate,
    mouse_action_move,
};

/**
 * @brief Containing different types of supported input devices
 */
enum n1_input_device_type_e {
    input_device_keyboard,
    input_device_mouse
};

/**
 * @brief Containing types of supported key states
 */
enum n1_key_state_e {
    key_state_released,
    key_state_pressed
};

/**
 * @brief Contains a process information
 */
typedef struct n1_process_s {
    char name[256];
    pid_t pid;
} n1_process_t;

/**
 * @brief Contains a memory region information
 */
typedef struct n1_region_s {
    pid_t pid;
    char name[256];
    uintptr_t start;
    uintptr_t end;
} n1_region_t;

/**
 * @brief Contains information for write ioctl
 */
typedef struct n1_rw_s {
    pid_t pid;
    uintptr_t address;
    size_t len;
    uintptr_t buffer_address;
} n1_rw_t;

/**
 * @brief Contains info about input generation
 */
typedef struct n1_input_s {
    int code;
    int value;
    int type;
    enum n1_input_device_type_e device_type;
} n1_input_t;

/**
 * @brief Contains info about key state
 */
typedef struct n1_key_state_s {
    int code;
    enum n1_key_state_e state;
} n1_key_state_t;

/**
 * @brief Contains information about a signture
 */
typedef struct n1_signature_s {
    char* signature;
    int relative;
    uintptr_t extra;
    uintptr_t offsets[16];
    size_t offsets_len;
    size_t size;
} n1_signature_t;