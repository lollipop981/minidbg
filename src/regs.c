#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>

int print_regs_command(pid_t pid){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("r15=0x%llx\nr14=0x%llx\nr13=0x%llx\nr12=0x%llx\nrbp=0x%llx\nrbx=0x%llx\nr11=0x%llx\nr10=0x%llx\nr9=0x%llx\nr8=0x%llx\nrax=0x%llx\nrcx=0x%llx\nrdx=0x%llx\nrsi=0x%llx\nrdi=0x%llx\norig_rax=0x%llx\nrip=0x%llx\ncs=0x%llx\neflags=0x%llx\nrsp=0x%llx\nss=0x%llx\nfs_base=0x%llx\ngs_base=0x%llx\nds=0x%llx\nes=0x%llx\nfs=0x%llx\ngs=0x%llx\n",
            regs.r15, regs.r14, regs.r13, regs.r12, regs.rbp, regs.rbx, regs.r11, regs.r10, regs.r9, regs.r8,
             regs.rax, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.orig_rax, regs.rip, regs.cs, regs.eflags,
              regs.rsp, regs.ss, regs.fs_base, regs.gs_base, regs.ds, regs.es, regs.fs, regs.gs);
}

int handle_regs_command(char *cmd, pid_t pid) {
    print_regs_command(pid);
}