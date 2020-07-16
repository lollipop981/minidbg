#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <sys/types.h>
#include <stdlib.h>

typedef struct command_handler {
    char *prefix;
    int (*handler)(char *, pid_t);
    char *help;
} command_handler_t;

int handle_command(char *cmd, pid_t pid);
int handle_help_command(char *cmd, pid_t pid);
int handle_shell_command(char *cmd, pid_t pid);

#endif // __COMMANDS_H__