#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

static inline int finish_output(FILE *output)
{
    int write_failed = ferror(output);
    int close_failed = fclose(output) == EOF;

    if (write_failed || close_failed) {
        fprintf(stderr, "Could not write output.txt\n");
        return 0;
    }

    return 1;
}

#endif