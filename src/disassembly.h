#ifndef __DISASSEMBLY_H__
#define __DISASSEMBLY_H__

#define MEMORY_READ_SIZE (75)
#define INSTRUCTION_BUFFER_SIZE (256)

int handle_disassembly_command(char *cmd, pid_t pid);

#endif // !__DISASSEMBLY_H__