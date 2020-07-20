#ifndef __REGS_H__
#define __REGS_H__

#include <sys/types.h>

#include "utils.h"

status handle_regs_command(char *cmd, pid_t pid);
u_int64_t get_instruction_pointer(pid_t pid);
status decrement_instruction_pointer(pid_t pid);

#endif // __REGS_H__