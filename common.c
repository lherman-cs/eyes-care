#include "common.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int is_root()
{
    return getuid() == 0;
}

void die(int err, const char *msg)
{
    fprintf(stderr, "Error (%d): %s\n", err, msg);
    exit(err);
}
