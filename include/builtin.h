#ifndef BUILTIN_H
#define BUILTIN_H

#include "comm_treatment.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef int (*BuiltinFunc)(char **args);

typedef struct Builtin{
    const char *name;
    BuiltinFunc func;
} Builtin;

int run_builtin(char **args);

#endif