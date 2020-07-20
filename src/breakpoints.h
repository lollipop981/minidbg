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

#endif // __BREAKPIONTS_H__