#include <linux/input-event-codes.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "../include/definitions.h"
#include "../include/linux/ioctl.h"

/**
 * @brief Generates a mouse input
 * @param code Mouse button code or, mouse movement code
 * @param action Type of action (see: n1_mouse_action_e)
 * @param value Number of units to move the cursor in case of a mouse movement event, unused otherwise
 */
void generate_mouse_input(int code, enum n1_mouse_action_e action, int value) {
    if (action == mouse_action_actuate || action == mouse_action_press) {
        generate_input(code, 1, EV_KEY, input_device_mouse);
    } else if (action == mouse_action_move) {
        generate_input(code, value, EV_REL, input_device_mouse);

        return;
    }

    if (action == mouse_action_actuate || action == mouse_action_release) {
        generate_input(code, 0, EV_KEY, input_device_mouse);
    }
}

/**
 * @brief Generates a keyboard input
 * @param code Key code 
 * @param action Type of action (see: n1_keyboard_action_e)
 */
void generate_keyboard_input(int code, enum n1_keyboard_action_e action) {
    if (action == keyboard_action_actuate || action == keyboard_action_press) {
        generate_input(code, 1, EV_KEY, input_device_keyboard);
    }

    if (action == keyboard_action_actuate || action == keyboard_action_release) {
        generate_input(code, 0, EV_KEY, input_device_keyboard);
    }
}

/**
 * @brief Write callback used by curl to write the downloaded file to disk
 * @param ptr pointer to the data
 * @param size size of each element
 * @param nmemb number of elements
 * @param stream file stream
 * @return number of bytes written
 */
size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

/**
 * @brief Downloads a file from the internet at the specified url and saves it to the specified path
 * @param url source url, should be a direct link to the file
 * @param path destination path (including file name)
 * @return 0 on success, 1 on failure
 */
int download_file(const char* url, const char* path) {
    CURL *curl;
    FILE *fp;
    CURLcode res = 1;

    curl = curl_easy_init();
    if (curl == NULL) {
        return 1;
    }

    fp = fopen(path, "wb");
    if (fp == NULL) {
        goto download_file_cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);
    fclose(fp);

download_file_cleanup:
    curl_easy_cleanup(curl);

    return res == CURLE_OK ? 0 : 1;
}

/**
 * @brief Generates a random string
 * @param len length of the string
 */
char* malloc_random_string(size_t len) {
    char chars[64] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_#";
    char* string = malloc(len + 1);

    for (size_t i = 0; i < len; i++) {     
        string[i] = chars[rand() % 64];
    }

    string[len] = '\0';

    return string;
}

/**
 * @brief Seeds the random number generator using microseconds
 */
void init_rand_seed(void) {
    struct timeval t1;

    gettimeofday(&t1, NULL);
    srand((unsigned int)(t1.tv_usec * t1.tv_sec));
}

/**
 * @brief Setup a callback function on signal detection
 * @param signal Signal (see: signum-generic.h)
 * @param handler Function pointer to the handler function
 */
void setup_signal_callback(int signal, void (*handler)(int)) {
    struct sigaction sigint_handler;

    sigint_handler.sa_handler = handler;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags = 0;

    sigaction(signal, &sigint_handler, NULL);
}

/**
 * @brief Reads the provided address and returns the address it points to. 
 * @param proc Process to read from
 * @param rip Original address
 * @param offset Offset to address in bytes
 * @param instruction_size Size of the whole instruction
 * @return Address it points to
 */
uintptr_t get_relative_address(n1_process_t* proc, uintptr_t rip, size_t offset, size_t instruction_size) {
    uint32_t relative_address;

    if (read_memory(proc, rip + offset, sizeof(uint32_t), &relative_address) != sizeof(uint32_t)) {
        return 0;
    }

    return rip + relative_address + instruction_size;
}

/**
 * @brief Reads the provided address and returns the address it points to, skipping the opcode.
 * @param proc Process to read from
 * @param address Address to get calling address from
 * @return Calling address
 */
uintptr_t get_call_address(n1_process_t* proc, uintptr_t address) {
    return get_relative_address(proc, address, 1, 5);
}

/**
 * @brief Scans a pattern of bytes and returns the relative offset in the region
 * @param process Process to search in
 * @param region Region of process containing the signature
 * @param signture Signature in IDA form (ex "4B 55 ? ? ? ? ? 55 90")
 * @return Memory address, 0 on failure
 */
uintptr_t get_signature_offset(n1_process_t* process, n1_region_t* region, const char* signature) {
    uint8_t mask[256] = {0};
    uint8_t pattern[256] = {0};
    size_t len = 0;
    uint8_t* buffer;
    size_t region_size = region->end - region->start;
    uintptr_t status = 0;

    for (size_t i = 0; signature[i] != '\0'; i++) {
        if (signature[i] == ' ') {
            continue;
        }

        if (signature[i] == '?') {
            mask[len] = 1;
            pattern[len] = 0;
            len += 1;
            continue;
        }

        mask[len] = 0;
        pattern[len] = (uint8_t)strtol(&signature[i], NULL, 16);
        len += 1;
        i++;
    }

    buffer = malloc(sizeof(uint8_t) * region_size);
    if (read_memory(process, region->start, region_size, buffer) != (ssize_t)region_size) {
        goto get_signature_address_free;
        return status;
    }

    for (size_t i = 0; i < region_size; i++) {
        int correct = 1;

        for (size_t j = 0; j < len; ++j) {
            correct = mask[j] || pattern[j] == buffer[i + j];

            if (!correct) {
                break;
            }
        }

        if (correct) {
            status = i;
            break;
        }
    }

get_signature_address_free:
    free(buffer);

    return status;
}

/**
 * @brief Dumps an offset from the running process at address
 * @param proc Process to search in
 * @param region Region of the process to search in
 * @param sig Signature information
 * @return Dumped offset
 */
uintptr_t get_offset(n1_process_t* proc, n1_region_t* region, n1_signature_t* sig) {
    uintptr_t offset = get_signature_offset(proc, region, sig->signature) + sig->offsets[0];
    offset += region->start;

    if (sig->relative) {
        offset = get_call_address(proc, offset);

        for (size_t i = 1; i < sig->offsets_len; ++i) {
            if (read_memory(proc, offset + sig->offsets[i], sizeof(uintptr_t), &offset) != sizeof(uintptr_t)) {
                return 0;
            }
        }
    } else {
        uintptr_t address;

        if (read_memory(proc, offset, sig->size, &address) != (long)sig->size) {
            return 0;
        }

        return address;
    }

    return offset + sig->extra;
}

/**
 * @brief Sleep in milliseconds
 * @param msec number of milliseconds to sleep
 * @return 
 */
int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
