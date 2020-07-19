#ifndef __MEM_H__
#define __MEM_H__

#include <sys/types.h>

#define READ_BUFFER_SIZE (4096)
#define MAX_PATH (4096)

int remote_memory_read(pid_t pid, long long unsigned address, size_t length, unsigned char *buffer, size_t buffer_size);
int handle_memory_read_command(char *cmd, pid_t pid);
int handle_maps_command(char *cmd, pid_t pid);
int get_min_executable_address(pid_t pid, long unsigned *address);
int poke_data(pid_t pid, uint64_t address, char data);

#endif // __MEM_H__