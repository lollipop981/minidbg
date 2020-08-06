#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>
#include <string.h>

#include "regs.h"

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

status print_all_regs(pid_t pid){
    if (-1 == ptrace(PTRACE_GETREGS, pid, NULL, &regs)) {
        return ERROR;
    }

    for (int i=0; i < reg_converter_length; i++) {
        printf("%s=0x%llx\n", reg_converter[i].reg_name, *(reg_converter[i].value));
    }

    return NO_ERROR;
}

status handle_regs_command(char *cmd, pid_t pid) {
    if (-1 == ptrace(PTRACE_GETREGS, pid, NULL, &regs)) {
        return ERROR;
    }
    if (*cmd != '\0') {
        for (int i = 0; i < reg_converter_length; i++) {
            if (0 == strcmp(reg_converter[i].reg_name, cmd)) {
                printf("%s=0x%llx\n", reg_converter[i].reg_name, *(reg_converter[i].value));
                return NO_ERROR;
            }
        }
        printf("unrecognized register \"%s\".\n", cmd);
        return ERROR;
    }

    return print_all_regs(pid);    
}

u_int64_t get_instruction_pointer(pid_t pid) {
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    return regs.rip;
}

status decrement_instruction_pointer(pid_t pid) {
    if (-1 == ptrace(PTRACE_GETREGS, pid, NULL, &regs)) {
        return ERROR;
    }

    regs.rip--;

    if (-1 == ptrace(PTRACE_SETREGS, pid, NULL, &regs)) {
        return ERROR;
    }

    return NO_ERROR;
}