#include <sys/uio.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "mem.h"
#include "utils.h"

unsigned char buffer[READ_BUFFER_SIZE] = { 0 };

int remote_memory_read(pid_t pid, long long unsigned address, size_t length, unsigned char *buffer, size_t buffer_size) {
    int ret = 0;
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = buffer_size;
    remote[0].iov_base = (void *) address;
    remote[0].iov_len = length;

    ret = process_vm_readv(pid, local, 1, remote, 1, 0);
    if (ret < 0) {
        printf("Error! ");
        switch (errno)
        {
        case EFAULT:
            printf("The memory is outside the accessible address space.\n");
            /* code */
            break;
        case EINVAL:
            printf("Invalid arguments for memory read.\n");
            break;
        case ENOMEM:
            printf("Could not allocate memory for operation.\n");
            break;
        case EPERM:
            printf("The caller does not have permission to access the address space of the process with pid %u.\n", pid);
            break;
        case ESRCH:
            printf("No process with pid %u.\n", pid);
            break;
        
        default:
            printf("Unexpected error!\n");
            break;
        }

        return 1;
    }
    return 0;
}

int handle_memory_read_command(char *cmd, pid_t pid) {
    int ret = 0;
    long long unsigned address = 0;
    size_t length = 0;
    

    memset(buffer, '\0', READ_BUFFER_SIZE);
    ret = sscanf(cmd, "%llx %lu", &address, &length);
    if (ret != 2) {
        printf("Invalid input! Usage: mem {addr} {count}\n");
    }

    if (length > READ_BUFFER_SIZE) {
        printf("Max allowed number of bytes to read is %u.\n", READ_BUFFER_SIZE);
    }

    ret = remote_memory_read(pid, address, length, buffer, READ_BUFFER_SIZE);
    if (ret > 0) {
        return 1;
    }

    hex_dump(buffer, length);   
    return 0;   
}

int handle_maps_command(char *cmd, pid_t pid) {
    char file_name[25] = { 0 };
    FILE *fp;
    int ret = 0;

    sprintf(file_name, "/proc/%d/maps", pid);
    fp = fopen(file_name, "r");
    if (NULL == fp) {
        printf("Error opening %s!\n", file_name);
        ret = 1;
        goto cleanup;
    }

    do {
        memset(buffer, '\0', READ_BUFFER_SIZE);
        fread(buffer, READ_BUFFER_SIZE, 1, fp);
        printf("%s", buffer);
    } while (!feof(fp));

cleanup:
    fclose(fp);
    return ret;
}