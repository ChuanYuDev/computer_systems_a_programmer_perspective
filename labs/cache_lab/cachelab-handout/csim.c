#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"


struct cache_line
{
    struct cache_line *ptr_prev;
    struct cache_line *ptr_next;
    long tag_valid_bit;
};

struct cache_line **initialize_cache(int num_set, int num_cahce_line)
{
    struct cache_line **cache_ptr = (struct cache_line **)malloc(sizeof(struct cache_line *) * num_set);
    struct cache_line *cache_line_ptr = NULL;

    for (int i = 0; i < num_set; i++)
    {
        cache_ptr[i] = (struct cache_line *)malloc(sizeof(struct cache_line) * num_cahce_line);
        cache_line_ptr = cache_ptr[i];

        cache_line_ptr->ptr_prev = NULL;

        for (int j = 1; j < num_cahce_line - 1; j++)
        {
            cache_line_ptr[j].ptr_prev = cache_line_ptr + (j - 1);
            cache_line_ptr[j].ptr_next = cache_line_ptr + (j + 1);
        }

        cache_line_ptr[num_cahce_line - 1].ptr_next = NULL;

        if (num_cahce_line > 1)
        {
            cache_line_ptr[num_cahce_line - 1].ptr_prev = cache_line_ptr + (num_cahce_line - 2);
        }
    }

    return cache_ptr;
}

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

    int num_set_bit = 2, num_cache_line = 2;
    int num_set = 1 << num_set_bit;
    // num_block_bit = 4;

    

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
    struct cache_line **cache_ptr = initialize_cache(num_set, num_cache_line);

    for (int i = 0; i < num_set; ++i)
    {
        free(cache_ptr[i]);
    }

    free(cache_ptr);

    /* Simulate */

    printSummary(0, 0, 0);
    return 0;
}
