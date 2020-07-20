#ifndef __MEM_H__
#define __MEM_H__

#include <sys/types.h>

#include "utils.h"

#define READ_BUFFER_SIZE (4096)
#define MAX_PATH (4096)

status remote_memory_read(pid_t pid, long long unsigned address, size_t length, unsigned char *buffer, size_t buffer_size);
status remote_memory_write(pid_t pid, long long unsigned address, unsigned char *buffer, size_t length);
status handle_memory_read_command(char *cmd, pid_t pid);
status handle_maps_command(char *cmd, pid_t pid);
status get_min_executable_address(pid_t pid, long unsigned *address);

#endif // __MEM_H__