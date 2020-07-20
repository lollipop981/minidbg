#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <sys/types.h>
#include <stdlib.h>

#include "utils.h"

typedef struct command_handler {
    char *prefix;
    status (*handler)(char *, pid_t);
    char *help;
} command_handler_t;

status handle_command(char *cmd, pid_t pid);
status handle_help_command(char *cmd, pid_t pid);
status handle_shell_command(char *cmd, pid_t pid);

#endif // __COMMANDS_H__