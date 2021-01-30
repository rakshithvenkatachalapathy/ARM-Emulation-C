/* utils.c - general utility functions for armemu */

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

/* malloc helper */
void *malloc_helper(size_t bytes, char *function_name)
{
    void *p;

    p = malloc(bytes);
    if (p == NULL) {
        printf("%s: malloc() failed, exiting.", function_name);
    }

    return p;
}