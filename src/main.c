#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define CMD_SIZE (400)

int main(int argc, char** argv)
{
        pid_t child_pid;
        char* debugee = argv[1];
        char cmd[CMD_SIZE] = { 0 };
             
        child_pid = fork();
        if (child_pid == 0) {
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            execlp(debugee, debugee, NULL);
        } else if (child_pid > 0) {
            int status;
            waitpid(child_pid, &status, 0);
            
            printf("rmdbg >>> ");
            fgets(cmd, CMD_SIZE, stdin);

            while (0 != strcmp("q\n", cmd)) {
                if (0 != strcmp("\n", cmd)) {
                    handle_command(cmd, child_pid);
                }
                memset(cmd, '\0', CMD_SIZE);
                printf("rmdbg >>> ");
                fgets(cmd, CMD_SIZE, stdin);

            } ;
  
            ptrace(PTRACE_CONT, child_pid, NULL, NULL);

        }
}  