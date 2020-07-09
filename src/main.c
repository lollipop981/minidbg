#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char** argv)
{
        pid_t child_pid;
        char* debugee = argv[1];
        child_pid = fork();

        if (child_pid == 0) {
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            execlp(debugee, debugee, NULL);
        } else if (child_pid > 0) {
            int status;

            waitpid(child_pid, &status, 0);
            ptrace(PTRACE_CONT, child_pid, NULL, NULL);


            // while (WIFSTOPPED(child_pid))
            // {
            //         struct user_regs_struct regs;
            //         ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
            //         unsigned instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);
            //         printf("EIP = 0x%08x, instr = 0x%08x\n", regs.rip, instr);

            //         ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0);

            //         wait(&child_pid);
            // }

        }
}  