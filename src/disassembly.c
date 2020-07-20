#include <stdio.h>
#include <inttypes.h>
#include <Zydis/Zydis.h>

#include "disassembly.h"
#include "mem.h"
#include "regs.h"
#include "symbols.h"

status disassemble_at_address(pid_t pid, uint64_t start_address) {
    ZyanU8 memory[MEMORY_READ_SIZE] = { 0 };
    ZyanU64 current_address = 0;
    ZyanU64 runtime_address = 0;
    ZyanUSize offset = 0;
    const ZyanUSize length = MEMORY_READ_SIZE;
    ZydisDecodedInstruction instruction;
    char buffer[INSTRUCTION_BUFFER_SIZE];

    runtime_address = get_instruction_pointer(pid);
    current_address = start_address;
    
    if (FAIL(remote_memory_read(pid, current_address, MEMORY_READ_SIZE, memory, MEMORY_READ_SIZE))) {
        printf("Failed reading memory at address %lx\n", current_address);
        return ERROR;
    }
    
    // Initialize decoder context
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);

    // Initialize formatter. Only required when you actually plan to do instruction
    // formatting ("disassembling"), like we do here
    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

    // Loop over the instructions in our buffer.
    while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, memory + offset, length - offset,
        &instruction))) {
        if (current_address == runtime_address) {
            printf("=> ");
        } else {
            printf("   ");
        }

        // Print current instruction pointer.
        printf("%016" PRIX64 "  ", current_address);

        // Format & print the binary instruction structure to human readable format
        ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
            current_address);
        puts(buffer);

        offset += instruction.length;
        current_address += instruction.length;
    }
    return NO_ERROR;
}

status handle_disassembly_command(char *cmd, pid_t pid) {
    uint64_t disas_address = 0;
    
    if (*cmd == '\0') {
        disas_address = get_instruction_pointer(pid);
    } else if (sscanf(cmd, "0x%lx", &disas_address) != 1) {
        if (get_symbol_address(cmd, pid, &disas_address) != 0) {
            printf("Failed to parse symbol or address: %s\n", cmd);
            return ERROR;
        }
    }

    return disassemble_at_address(pid, disas_address);
}