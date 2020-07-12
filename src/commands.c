#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "regs.h"
#include "mem.h"

command_handler_t COMMAND_HANDLERS[] = {
    {"help", handle_help_command, "Print Help."},
    {"reg", handle_regs_command, "Print information about registers."},
    {"mem", handle_memory_read_command, "Print Memory from the debugee. Usage: mem {addr} {count}."},
    
};
size_t COMMAND_HANDLER_COUNT = sizeof(COMMAND_HANDLERS) / sizeof(command_handler_t);

int handle_command(char *cmd, pid_t pid){
    char *prefix = NULL;
    size_t len = 0;

    // remove new line
    if (cmd[strlen(cmd) - 1] == '\n') {
        cmd[strlen(cmd) - 1] = '\0';
    }

    for (int i = 0; i < COMMAND_HANDLER_COUNT; i++) {
        prefix = COMMAND_HANDLERS[i].prefix;
        if (strlen(prefix) <= strlen(cmd)) {
            if (0 == memcmp(cmd, prefix, strlen(prefix))) {
                // send only the rest of the cmd.
                cmd += strlen(prefix);
                while (*cmd == ' ') {
                    cmd++;
                }

                return COMMAND_HANDLERS[i].handler(cmd, pid);
            }
        }
        
    }


    printf("Undefined command: \"%s\". Try \"help\"\n", cmd);
}

int handle_help_command(char *cmd, pid_t pid) {
    command_handler_t *handler = NULL;

    for (int i = 0; i < COMMAND_HANDLER_COUNT; i++) {
        handler = &(COMMAND_HANDLERS[i]);
        printf("%s -- %s\n", handler->prefix, handler->help);
    }
}