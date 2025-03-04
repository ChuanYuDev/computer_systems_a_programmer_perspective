#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

int main(int argc, char *argv[])
{
    /* Parse the arguments */


    // int flags, opt;
    // int nsecs, tfnd;

    // nsecs = 0;
    // tfnd = 0;
    // flags = 0;
    // while ((opt = getopt(argc, argv, "nt:")) != -1) {
    //     switch (opt) {
    //     case 'n':
    //         flags = 1;
    //         break;
    //     case 't':
    //         nsecs = atoi(optarg);
    //         tfnd = 1;
    //         break;
    //     default: /* '?' */
    //         fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
    //                 argv[0]);
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // printf("flags=%d; tfnd=%d; nsecs=%d; optind=%d\n",
    //         flags, tfnd, nsecs, optind);

    // if (optind >= argc) {
    //     fprintf(stderr, "Expected argument after options\n");
    //     exit(EXIT_FAILURE);
    // }

    // printf("name argument = %s\n", argv[optind]);

    // /* Other code omitted */

    // exit(EXIT_SUCCESS);

    // int s = 2, E = 1, b = 4;

    /* Read the trace file */

    char *filename = "traces/dave.trace";
    char line[256];

    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        char op;
        long address;
        int size;

        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, " %c %ld,%d", &op, &address, &size);
            printf("%s", line);

            printf("%c - %ld - %d\n", op, address, size);
        }

        fclose(file);
    }


    /* Initialize the cache */

    /* Simulate */

    printSummary(0, 0, 0);
    return 0;
}
