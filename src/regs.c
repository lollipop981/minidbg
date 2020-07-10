#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>

int handle_regs_command(char *cmd, pid_t pid) {
    print_regs_command(pid);
}

int print_regs_command(pid_t pid){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("r15=0x%x\nr14=0x%x\nr13=0x%x\nr12=0x%x\nrbp=0x%x\nrbx=0x%x\nr11=0x%x\nr10=0x%x\nr9=0x%x\nr8=0x%x\nrax=0x%x\nrcx=0x%x\nrdx=0x%x\nrsi=0x%x\nrdi=0x%x\norig_rax=0x%x\nrip=0x%x\ncs=0x%x\neflags=0x%x\nrsp=0x%x\nss=0x%x\nfs_base=0x%x\ngs_base=0x%x\nds=0x%x\nes=0x%x\nfs=0x%x\ngs=0x%x\n",
            regs.r15, regs.r14, regs.r13, regs.r12, regs.rbp, regs.rbx, regs.r11, regs.r10, regs.r9, regs.r8,
             regs.rax, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.orig_rax, regs.rip, regs.cs, regs.eflags,
              regs.rsp, regs.ss, regs.fs_base, regs.gs_base, regs.ds, regs.es, regs.fs, regs.gs);
}
