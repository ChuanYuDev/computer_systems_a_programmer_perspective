#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

static int num_set_bit, num_cache_line, num_block_bit, verbose;
static int hit_count, miss_count, eviction_count;

struct cache_line
{
    struct cache_line *ptr_prev;
    struct cache_line *ptr_next;
    unsigned long tag_valid_bit;
};

void free_cache(struct cache_line **cache_ptr, int num_set)
{
    struct cache_line *line_ptr = NULL, *line_ptr_next = NULL;

    for (int i = 0; i < num_set; ++i)
    {
        line_ptr = cache_ptr[i];

        while (line_ptr)
        {
            line_ptr_next = line_ptr->ptr_next;
            free((void *)line_ptr);

            line_ptr = line_ptr_next;
        }
    }

    free((void *) cache_ptr);
}

struct cache_line **initialize_cache(int num_set)
{
    struct cache_line **cache_ptr = (struct cache_line **)calloc(num_set, sizeof(struct cache_line *));

    if (!cache_ptr)
        return NULL;

    struct cache_line *line_ptr = NULL;
    struct cache_line *line_ptr_prev = NULL;

    for (int i = 0; i < num_set; i++)
    {
        line_ptr_prev = (struct cache_line *)malloc(sizeof(struct cache_line));

        /* If allcation is failed, the blocks already allocated will be freed */
        if (!line_ptr_prev)
        {
            free_cache(cache_ptr, num_set);

            return NULL;
        }

        line_ptr_prev->ptr_prev = NULL;
        line_ptr_prev->tag_valid_bit = 0;

        cache_ptr[i] = line_ptr_prev;

        for (int j = 1; j < num_cache_line; j++)
        {
            line_ptr = (struct cache_line *)malloc(sizeof(struct cache_line));

            if (!line_ptr)
            {
                line_ptr_prev->ptr_next = NULL;
                free_cache(cache_ptr, num_set);

                return NULL;
            }

            line_ptr_prev->ptr_next = line_ptr;
            line_ptr->ptr_prev = line_ptr_prev;
            line_ptr->tag_valid_bit = 0;

            line_ptr_prev = line_ptr;
        }

        line_ptr_prev->ptr_next = NULL;
    }

    return cache_ptr;
}

/* Move the node first, make it the most recently used */
void move_first(struct cache_line **set_ptr, struct cache_line *line_ptr)
{
    if (!line_ptr->ptr_prev)
        return;
    
    /* Handle the line previous and next line relation */
    line_ptr->ptr_prev->ptr_next = line_ptr->ptr_next;

    if (line_ptr->ptr_next)
        line_ptr->ptr_next->ptr_prev = line_ptr->ptr_prev;
    
    /* Handle the line and the first line relation */
    line_ptr->ptr_next = *set_ptr;
    (*set_ptr)->ptr_prev = line_ptr;

    /* Handle the line and cache array relation */
    line_ptr->ptr_prev = NULL;
    *set_ptr = line_ptr;

    return;
}

void simulate(struct cache_line **cache_ptr, unsigned long address)
{
    int set_index = (address >> num_block_bit) & (~((-1) << num_set_bit));
    unsigned tag_bit = address >> (num_set_bit + num_block_bit);

    int valid_bit = 0;

    struct cache_line *line_ptr = cache_ptr[set_index];
    struct cache_line *line_lru_ptr = NULL;

    while(line_ptr)
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

        if (!line_ptr->ptr_next)
        {
            line_lru_ptr = line_ptr;
        }
        
        line_ptr = line_ptr->ptr_next;
    }

    if (!line_ptr)
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
    int opt;
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "vs:E:b:t:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = 1;
                break;

            case 's':
                num_set_bit = atoi(optarg);
                break;
            
            case 'E':
                num_cache_line = atoi(optarg);
                break;
            
            case 'b':
                num_block_bit = atoi(optarg);
                break;
            
            case 't':
                filename = optarg;
                break;

            default: /* '?' */
                fprintf(stderr, "Usage: %s [-v] -s <s> -E <E> -b <b> -t <tracefile> \n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    int num_set = 1 << num_set_bit;

    /* Initialize the cache */
    struct cache_line **cache_ptr = initialize_cache(num_set);

    if (!cache_ptr)
        return EXIT_FAILURE;

    /* Read the trace file */
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

            switch(op)
            {
                case 'I':
                    continue;
                    break;
                
                case 'M': 
                case 'L':
                case 'S':

                    if (verbose)
                        printf("%c %lx,%d", op, address, size);

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
    free_cache(cache_ptr, num_set);

    /* Print summary */
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
