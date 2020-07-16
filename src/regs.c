#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>
#include <string.h>

struct user_regs_struct regs;

struct string_to_reg {
    char *reg_name;
    __extension__ unsigned long long int *value;
};

struct string_to_reg reg_converter[] = {
  { "r15", &regs.r15 },
  { "r14", &regs.r14 },
  { "r13", &regs.r13 },
  { "r12", &regs.r12 },
  { "rbp", &regs.rbp },
  { "rbx", &regs.rbx },
  { "r11", &regs.r11 },
  { "r10", &regs.r10 },
  { "r9", &regs.r9 },
  { "r8", &regs.r8 },
  { "rax", &regs.rax },
  { "rcx", &regs.rcx },
  { "rdx", &regs.rdx },
  { "rsi", &regs.rsi },
  { "rdi", &regs.rdi },
  { "orig_rax", &regs.orig_rax },
  { "rip", &regs.rip },
  { "cs", &regs.cs },
  { "eflags", &regs.eflags },
  { "rsp", &regs.rsp },
  { "ss", &regs.ss },
  { "fs_base", &regs.fs_base },
  { "gs_base", &regs.gs_base },
  { "ds", &regs.ds },
  { "es", &regs.es },
  { "fs", &regs.fs },
  { "gs", &regs.gs },
};
size_t reg_converter_length = sizeof(reg_converter) / sizeof(struct string_to_reg);

int print_all_regs(pid_t pid){
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("r15=0x%llx\nr14=0x%llx\nr13=0x%llx\nr12=0x%llx\nrbp=0x%llx\nrbx=0x%llx\nr11=0x%llx\nr10=0x%llx\nr9=0x%llx\nr8=0x%llx\nrax=0x%llx\nrcx=0x%llx\nrdx=0x%llx\nrsi=0x%llx\nrdi=0x%llx\norig_rax=0x%llx\nrip=0x%llx\ncs=0x%llx\neflags=0x%llx\nrsp=0x%llx\nss=0x%llx\nfs_base=0x%llx\ngs_base=0x%llx\nds=0x%llx\nes=0x%llx\nfs=0x%llx\ngs=0x%llx\n",
            regs.r15, regs.r14, regs.r13, regs.r12, regs.rbp, regs.rbx, regs.r11, regs.r10, regs.r9, regs.r8,
             regs.rax, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.orig_rax, regs.rip, regs.cs, regs.eflags,
              regs.rsp, regs.ss, regs.fs_base, regs.gs_base, regs.ds, regs.es, regs.fs, regs.gs);
}

int handle_regs_command(char *cmd, pid_t pid) {
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    if (*cmd != '\0') {
        for (int i = 0; i < reg_converter_length; i++) {
            if (0 == strcmp(reg_converter[i].reg_name, cmd)) {
                printf("%s=0x%llx\n", reg_converter[i].reg_name, *(reg_converter[i].value));
                return 0;
            }
        }
        printf("unrecognized register \"%s\".\n", cmd);
        return 1;
    } else {
        print_all_regs(pid);
    }
}

u_int64_t get_instruction_pointer(pid_t pid) {
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    return regs.rip;
}