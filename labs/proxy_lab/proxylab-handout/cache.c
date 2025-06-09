#include "cache.h"

/* Object helper function */
void obj_init(obj_t *obj_ptr)
{
    obj_ptr->size = 0;
}

/* 
 * obj_deinit
 * free obj.buf then free obj, obj itself is dynamically allocated
 */
void obj_deinit(obj_t *obj_ptr)
{
    free(obj_ptr->buf);
    free(obj_ptr);
}

/* Cache helper function */
void cache_init(cache_t *cache_ptr)
{
    cache_ptr->first_obj_ptr = NULL;
    cache_ptr->last_obj_ptr = NULL;

    cache_ptr->size = 0;

    cache_ptr->read_cnt = 0;
    Sem_init(&cache_ptr->mutex, 1);
    Sem_init(&cache_ptr->w, 1);
}

void cache_deinit(cache_t *cache_ptr)
{
    obj_t *obj_ptr = cache_ptr->first_obj_ptr;

    if (!obj_ptr)
        return;
    
    obj_t *obj_ptr_next;

    do
    {
        obj_ptr_next = obj_ptr->next;

        obj_deinit(obj_ptr);

        obj_ptr = obj_ptr_next;
    } while (obj_ptr);
    
    return;
}

/*
 * cache_read: read cache to find the first object with request line equal to one pointed by rl_ptr
 * Implement first readers-writers problem
 */
obj_t * cache_read(cache_t *cache_ptr, request_line_t *rl_ptr)
{
    P(&cache_ptr->mutex);
    cache_ptr->read_cnt++;

    if (cache_ptr->read_cnt == 1)
        P(&cache_ptr->w);
    
    V(&cache_ptr->mutex);

    /* Critical section, reading happens */
    // TO DO: read obj with rl_ptr

    P(&cache_ptr->mutex);
    cache_ptr->read_cnt--;

    if (cache_ptr->read_cnt == 0)
        V(&cache_ptr->w);
    
    V(&cache_ptr->mutex);
}

/*
 * cache_move_first and cache_insert both belong to writer function
 * Implement first readers-writers problem
 */
void cache_move_first(cache_t *cache_ptr, obj_t *obj_ptr)
{
    P(&cache_ptr->w);

    /* Critical section, writing happens */
    // TO DO: move first obj_ptr

    V(&cache_ptr->w);
}

void cache_insert(cache_t *cache_ptr, obj_t *obj_ptr)
{
    P(&cache_ptr->w);

    /* Critical section, writing happens */
    // TO DO: insert obj_ptr

    V(&cache_ptr->w);
}
