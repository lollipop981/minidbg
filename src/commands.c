#include <sys/types.h>

#include "commands.h"
#include "regs.h"

command_handler_t COMMAND_HANDLERS[] = {
    {"reg", handle_regs_command, "Print information about registers\n"},
};
size_t command_handler_count = 1;

void handle_command(char *command, pid_t pid){
    if (strcmp(command, "getregs") == 0){
        get_regs_command(pid);
    }
}
