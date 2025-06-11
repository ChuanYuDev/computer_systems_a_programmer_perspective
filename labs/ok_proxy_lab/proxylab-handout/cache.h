#ifndef __CACHE_H__
#define __CACHE_H__

#include "helper.h"

/* Cache for storing web objects */
typedef struct obj_type
{
    void *buf;
    request_line_t rl;  /* Use rl_copy and is_rl_equal function to compare object */

    struct obj_type *pre;
    struct obj_type *next;

    int size;   /* Limited by MAX_OBJECT_SIZE */
} obj_t;

typedef struct
{
    obj_t *first_obj_ptr;
    obj_t *last_obj_ptr;

    int size;   /* Limited by MAX_CACHE_SIZE */

    /* For first readers-writers problem */
    int read_cnt;   /* Initially = 0 */           
    sem_t mutex, w; /* Both initially = 1 */
} cache_t;

/* Object helper function */
obj_t *obj_init(char *object_buf, int object_size, request_line_t *rl_ptr);
void obj_deinit(obj_t *obj_ptr);

/* Cache helper function */
void cache_init(cache_t *cache_ptr);
void cache_deinit(cache_t *cache_ptr);
void cache_print(cache_t *cache_ptr);
obj_t *cache_read(cache_t *cache_ptr, request_line_t *rl_ptr);
void cache_move_first(cache_t *cache_ptr, obj_t *obj_ptr);
void cache_evict_last(cache_t *cache_ptr);
void cache_insert(cache_t *cache_ptr, obj_t *obj_ptr);

#endif