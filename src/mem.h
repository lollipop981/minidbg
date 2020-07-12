#ifndef __MEM_H__
#define __MEM_H__

#include <sys/types.h>

#define READ_BUFFER_SIZE (4096)

int handle_memory_read_command(char *cmd, pid_t pid);

#endif // __MEM_H__