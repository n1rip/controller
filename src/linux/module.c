#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

extern long init_module(void *, unsigned long, const char *);
extern long delete_module(const char *, unsigned int);

/**
 * Loads a kernel module into the kernel
 * @param path Path to the compiled kernel module
 * @return 0 on success, 1 otherwise
 */
int load_module(const char* path) {
    int fd;
    void* image;
    off_t image_size;

    fd = open(path, O_RDONLY);
    if (fd == 0) {
        return 1;
    }

    image_size = lseek(fd, 0, SEEK_END);
    if (image_size == - 1) {
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) != 0) {
        close(fd);
        return 1;
    }

    image = malloc((size_t)image_size);

    if (read(fd, image, (size_t)image_size) != image_size) {
        free(image);
        close(fd);
        return 1;
    }

    if (init_module(image, (size_t)image_size, "") != 0) {
        free(image);
        close(fd);
        return 1;
    }

    free(image);
    close(fd);
    
    return 0;
}

/**
 * Unloads a kernel module
 * @param name Name of the kernel module to unload
 * @return 0 on success, 1 otherwise
 */
int remove_module(const char* name) {
    if (delete_module(name, O_NONBLOCK) != 0) {
		return 1;
	}

    return 0;
}
