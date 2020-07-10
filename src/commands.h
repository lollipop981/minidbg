#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <sys/types.h>

typedef struct command_handler {
    char *prefix;
    int (*handler)(char *, pid_t);
    char *help;
} command_handler_t;

void handle_command(char *command, pid_t pid);

#endif // __COMMANDS_H__