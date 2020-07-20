#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <sys/types.h>
#include <stdint.h>

#include "utils.h"

void syms_init(char *debugee);
status get_symbol_address(char *symbol, pid_t pid, uint64_t *address);

#endif // __SYMBOLS_H__