#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "symbols.h"
#include "mem.h"

char *elf_path = NULL;

void syms_init(char *debugee) {
    char *end = strchr(debugee, ' ');

    if (end == NULL) {
        elf_path = debugee;
    } else {
        elf_path = malloc(end - debugee + 1);
        if (elf_path != NULL) {
            strcpy(elf_path, debugee);
        }
    }
}

int get_symbol_offset(char *symbol, unsigned long *offset) {
    Elf         *elf;
    Elf_Scn     *scn = NULL;
    GElf_Shdr   shdr;
    Elf_Data    *data;
    int         fd, ii, count;
    GElf_Sym sym;
    char *symbol_name;
    int ret = 0;
    
    elf_version(EV_CURRENT);

    if (elf_path == NULL) {
        ret = 1;
        goto cleanup;
    }

    fd = open(elf_path, O_RDONLY);
    elf = elf_begin(fd, ELF_C_READ, NULL);

    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB) {
            /* found a symbol table */
            break;
        }
    }

    // symbol table not found
    if (scn == NULL) {
        ret = 1;
        goto cleanup;
    }

    data = elf_getdata(scn, NULL);
    count = shdr.sh_size / shdr.sh_entsize;

    /* print the symbol names */
    for (ii = 0; ii < count; ++ii) {
        
        gelf_getsym(data, ii, &sym);
        symbol_name = elf_strptr(elf, shdr.sh_link, sym.st_name);
        if (strcmp(symbol_name, symbol) == 0) {
            *offset = sym.st_value;
            break;
        }
    }

cleanup:
    elf_end(elf);
    close(fd);
    return ret;
}

int get_symbol_address(char *symbol, pid_t pid, uint64_t *address) {
    uint64_t min_address = 0;
    uint64_t offset = 0;

    if (get_symbol_offset(symbol, &offset)) {
        printf("Error finding symbol in symbol table.\n");
        return 1;
    }

    if (get_min_executable_address(pid, &min_address)) {
        printf("Error getting min executable address.\n");
        return 1;
    }

    if (offset == 0 || min_address == 0) {
        printf("Error finding symbol\n");
        return 1;
    }

    *address = min_address + offset;
    return 0;
}