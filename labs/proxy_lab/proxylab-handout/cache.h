#ifndef __CACHE_H__
#define __CACHE_H__

#include "helper.h"

#define KEY_BUFSIZE 8192
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
void obj_init(obj_t *obj_ptr);
void obj_deinit(obj_t *obj_ptr);

/* Cache helper function */
void cache_init(cache_t *cache_ptr);
void cache_deinit(cache_t *cache_ptr);


#endif