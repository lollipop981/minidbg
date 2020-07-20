#ifndef __BREAKPIONTS_H__
#define __BREAKPIONTS_H__

#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

#define INT3 ('\xcc')

typedef struct breakpoint {
    uint64_t address;
    unsigned char opcode;
    int is_enabled;
} breakpoint_t;

status handle_breakpoint_command(char *cmd, pid_t pid);
status handle_continue_command(char *cmd, pid_t pid);
void remove_breakpoints_for_disassembly(uint64_t start_address, unsigned char *buffer, size_t length);

#endif // __BREAKPIONTS_H__