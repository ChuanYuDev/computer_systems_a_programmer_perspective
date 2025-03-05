#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

static int num_set_bit, num_cache_line, num_block_bit, verbose;
static int hit_count = 0, miss_count = 0, eviction_count = 0;

struct cache_line
{
    struct cache_line *ptr_prev;
    struct cache_line *ptr_next;
    unsigned long tag_valid_bit;
};

struct cache_line **initialize_cache(int num_set, int num_cahce_line)
{
    struct cache_line **cache_ptr = (struct cache_line **)malloc(sizeof(struct cache_line *) * num_set);
    struct cache_line *cache_line_ptr = NULL;

    for (int i = 0; i < num_set; i++)
    {
        cache_ptr[i] = (struct cache_line *)malloc(sizeof(struct cache_line) * num_cahce_line);
        cache_line_ptr = cache_ptr[i];

        /* initialize ptr_prev, ptr_next */
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

        /* initialize tag bit, valid bit */

        for (int j = 0; j < num_cache_line; j++)
        {
            cache_line_ptr[j].tag_valid_bit = 0;
        }
    }

    return cache_ptr;
}

/* Move the node first, make it the most recently used */
void move_first(struct cache_line **set_ptr, struct cache_line *line_ptr)
{
    if (line_ptr->ptr_prev == NULL)
        return;
    
    line_ptr->ptr_prev->ptr_next = line_ptr->ptr_next;

    if (line_ptr->ptr_next != NULL)
        line_ptr->ptr_next->ptr_prev = line_ptr->ptr_prev;
    
    line_ptr->ptr_prev = NULL;
    line_ptr->ptr_next = *set_ptr;
    *set_ptr = line_ptr;

    return;
}

void simulate(struct cache_line **cache_ptr, unsigned long address)
{
    int set_index = (address >> num_block_bit) & (~((-1) << num_set_bit));
    unsigned tag_bit = address >> (num_set_bit + num_block_bit);
    // printf("%d\n", set_index);

    int valid_bit = 0;

    struct cache_line *line_ptr = cache_ptr[set_index];
    struct cache_line *line_lru_ptr = NULL;

    while(line_ptr != NULL)
    {
        valid_bit = line_ptr->tag_valid_bit & 1;

        if (!valid_bit)
        {
            miss_count++;

            if (verbose)
                printf(" miss");

            line_ptr->tag_valid_bit = (tag_bit << 1) + 1;
            break;
        }
        else
        {
            if (tag_bit == (line_ptr->tag_valid_bit >> 1))
            {
                hit_count++;

                if (verbose)
                    printf(" hit");

                break;
            }
        }

        if (line_ptr->ptr_next == NULL)
        {
            line_lru_ptr = line_ptr;
        }
        
        line_ptr = line_ptr->ptr_next;
    }

    if (line_ptr == NULL)
    {
        miss_count++;
        eviction_count++;

        if (verbose)
            printf(" miss eviction");

        line_lru_ptr->tag_valid_bit = (tag_bit << 1) + 1;

        move_first(cache_ptr + set_index, line_lru_ptr);

        return;
    }

    move_first(cache_ptr + set_index, line_ptr);
    return;
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

    num_set_bit = 4, num_cache_line = 2, num_block_bit = 4;
    verbose = 1;
    int num_set = 1 << num_set_bit;

    /* Initialize the cache */
    struct cache_line **cache_ptr = initialize_cache(num_set, num_cache_line);

    /* Read the trace file */
    char *filename = "traces/yi.trace";
    char line[256];

    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        char op;
        unsigned long address;
        int size;

        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, " %c %lx,%d", &op, &address, &size);

            if (verbose)
                printf("%c %lx,%d", op, address, size);

            switch(op)
            {
                case 'I':
                    continue;
                    break;
                
                case 'M': 
                case 'L':
                case 'S':

                    /* Simulate */
                    simulate(cache_ptr, address);
                    break;
                
                default:
                    fprintf(stderr, "Wrong operation: %c\n", op);
            }

            if (op == 'M')
            {
                hit_count++;

                if (verbose)
                    printf(" hit");
            }

            if (verbose)
                printf("\n");
        }

        fclose(file);
    }

    /* Free cache */
    for (int i = 0; i < num_set; ++i)
    {
        free(cache_ptr[i]);
    }

    free(cache_ptr);

    /* Print summary */
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
