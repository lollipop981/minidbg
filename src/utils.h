#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

typedef int status;

#define NO_ERROR (0)
#define ERROR (-1)
#define ERROR_SHOULD_EXIT (-2)

#define SUCCESS(function)       ((function) == NO_ERROR)
#define FAIL(function)          ((function) < NO_ERROR)

void hex_dump(const void* data, size_t size);

#endif // __UTILS_H__