#include <glib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "breakpoints.h"
#include "mem.h"

GList *g_breakpoints = NULL;

status list_breakpoints() {
    if (g_breakpoints == NULL) {
        printf("No breakpoints!\n");
        return NO_ERROR;
    }

    unsigned counter = 0;
    GList *br = g_breakpoints;
    breakpoint_t *current = NULL;

    do {
        current = br->data;
        printf("br %u: 0x%lx opcode: %02X enabled: %u\n", counter, current->address, current->opcode, current->is_enabled);
    } while ((br = g_list_next(br)) != NULL);
    
    return NO_ERROR;
}

status set_breakpoint(pid_t pid, uint64_t address) {
    unsigned char opcode;
    uint64_t data;
    unsigned char *converter;

    if ((data = ptrace(PTRACE_PEEKDATA, pid, address, 0)) == -1) {
        printf("Could no read old opcode at: 0x%lx\n", address);
        return ERROR;
    }

    converter = (void*)&data;
    opcode = *converter;
    *converter = INT3;
    
    if (-1 == ptrace(PTRACE_POKEDATA, pid, address, data)) {
        printf("Could not set breakpoint at: 0x%lx", address);
        return 1;
    }

    breakpoint_t *new_breakpoint = malloc(sizeof(breakpoint_t));
    if (new_breakpoint == NULL) {
        printf("Error allocating memory!\n");
        return ERROR;
    }

    new_breakpoint->address = address;
    new_breakpoint->opcode = opcode;
    new_breakpoint->is_enabled = 1;    
    g_breakpoints = g_list_append(g_breakpoints, new_breakpoint);
    return NO_ERROR;
}

status handle_breakpoint_command(char *cmd, pid_t pid) {
    if ('\0' == *cmd) {
        return list_breakpoints();
    }

    uint64_t address;
    if (sscanf(cmd, "0x%lx", &address) != 1) {
        printf("%s is not a valid address!\n", cmd);
        return ERROR;
    }

    return set_breakpoint(pid, address);
}

status handle_continue_command(char *cmd, pid_t pid) {
    int child_status;
    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &child_status, 0);
    if (WIFEXITED(child_status)) {
        printf("child exited\n");
        return ERROR_SHOULD_EXIT;
    } else if (WIFSTOPPED(child_status)) {
        printf("child stopped\n");
    } else {
        printf("status is: %d\n", child_status);
    }

    return NO_ERROR;
}