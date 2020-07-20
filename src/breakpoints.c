#include <glib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <errno.h>

#include "breakpoints.h"
#include "mem.h"
#include "regs.h"

GList *g_breakpoints = NULL;
int reached_breakpoint = FALSE;

status list_breakpoints() {
    if (g_breakpoints == NULL) {
        printf("No breakpoints!\n");
        return NO_ERROR;
    }

    unsigned counter = 0;
    GList *br = g_breakpoints;
    breakpoint_t *current = NULL;

    do {
        counter++;
        current = br->data;
        printf("br %u: 0x%lx opcode: %02X enabled: %u\n", counter, current->address, current->opcode, current->is_enabled);
    } while ((br = g_list_next(br)) != NULL);
    
    return NO_ERROR;
}

status replace_byte(pid_t pid, uint64_t address, unsigned char byte_to_put, unsigned char *old_byte) {
    uint64_t data;
    unsigned char *converter;
    int err = 0;

    errno = NO_ERROR;
    data = ptrace(PTRACE_PEEKDATA, pid, address, 0);
    if (errno != NO_ERROR) {
        err = errno;
        printf("Could not read old opcode at: 0x%lx, errno: %d\n", address, err);
        return ERROR;
    }

    converter = (void*)&data;
    *old_byte = *converter;
    *converter = byte_to_put;
    
    if (-1 == ptrace(PTRACE_POKEDATA, pid, address, data)) {
        err = errno;
        printf("Could not set breakpoint at: 0x%lx, errno: %d\n", address, err);
        return ERROR;
    }

    return NO_ERROR;
}

status set_breakpoint(pid_t pid, uint64_t address) {
    unsigned char opcode;
    
    if (FAIL(replace_byte(pid, address, INT3, &opcode))) {
        printf("Could not place breakpoint!\n");
        return ERROR;
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

status continue_with_breakpoint(pid_t pid) {
    unsigned char old_opcode;
    unsigned char output;
    int child_status;
    uint64_t address = get_instruction_pointer(pid);
    GList *breakpoints = g_breakpoints;
    breakpoint_t *current_breakpoint;
    
    while (breakpoints != NULL) {
        current_breakpoint = breakpoints->data;
        if (current_breakpoint->address == address) {
            break;
        }
        breakpoints = g_list_next(breakpoints);
    }
    
    if (breakpoints == NULL) {
        printf("Couldn't find breakpoint at 0x%lx\n", address);
        return ERROR;
    }

    // put back the old opcode, single step and place the breakpoint again.
    old_opcode = current_breakpoint->opcode;
    if (FAIL(replace_byte(pid, address, old_opcode, &output))) {
        return ERROR;
    }

    if (-1 == ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL)) {
        return ERROR;
    }

    waitpid(pid, &child_status, 0);

    if (FAIL(replace_byte(pid, address, INT3, &output))) {
        return ERROR;
    }

    if (output != old_opcode) {
        return ERROR;
    }

    return NO_ERROR;
}

status handle_continue_command(char *cmd, pid_t pid) {
    int child_status;
    if (reached_breakpoint) {
        // continue with breakpoint
        if (FAIL(continue_with_breakpoint(pid))) {
            printf("Error while reverting breakpoint, this may cause unexpected behaviour!\n");
        }
    }
    
    reached_breakpoint = FALSE;
    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &child_status, 0);

    if (WIFEXITED(child_status)) {
        printf("child exited\n");
        return ERROR_SHOULD_EXIT;
    } else if (WIFSTOPPED(child_status)) {
        if (SIGTRAP == WSTOPSIG(child_status)) {
            reached_breakpoint = TRUE;
            decrement_instruction_pointer(pid);
            printf("reached breakpoint at 0x%lx\n", get_instruction_pointer(pid));
        } else {
            printf("child stopped\n");
        }
    } else {
        printf("status is: %d\n", child_status);
    }

    return NO_ERROR;
}

void remove_breakpoints_for_disassembly(uint64_t start_address, unsigned char *buffer, size_t length) {
    uint64_t end_address = start_address + length;
    GList *breakpoints = g_breakpoints;
    breakpoint_t *current;

    while (breakpoints != NULL) {
        current = breakpoints->data;
        if (start_address <= current->address && current->address <= end_address) {
            buffer[current->address - start_address] = current->opcode;
        }

        breakpoints = g_list_next(breakpoints);
    }

}