#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <sys/types.h>
#include <stdint.h>

void syms_init(char *debugee);
void syms(pid_t pid);
int get_symbol_address(char *symbol, pid_t pid, uint64_t *address);

#endif // __SYMBOLS_H__