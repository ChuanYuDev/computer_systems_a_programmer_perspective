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
 * cache_print belongs to reader function
 *
 * Implement first readers-writers problem
 */
void cache_print(cache_t *cache_ptr)
{
    P(&cache_ptr->mutex);
    cache_ptr->read_cnt++;

    if (cache_ptr->read_cnt == 1)
        P(&cache_ptr->w);
    
    V(&cache_ptr->mutex);

    printf("Cache size: %d\n", cache_ptr->size);

    obj_t *obj_ptr = cache_ptr->first_obj_ptr;

    while (obj_ptr)
    {
        rl_print(&obj_ptr->rl);
        printf("Object size: %d\n", obj_ptr->size);

        obj_ptr = obj_ptr->next;
    }

    P(&cache_ptr->mutex);
    cache_ptr->read_cnt--;

    if (cache_ptr->read_cnt == 0)
        V(&cache_ptr->w);
    
    V(&cache_ptr->mutex);
}

/*
 * cache_read
 *  belongs to reader function
 *  reads cache to find the first object with request line equal to one pointed by rl_ptr
 *
 *  Implement first readers-writers problem
 * 
 * Return value:
 *  NULL, if no cache matched with rl_ptr
 *  First obj ptr, if any cache matched with rl_ptr
 */
obj_t * cache_read(cache_t *cache_ptr, request_line_t *rl_ptr)
{
    P(&cache_ptr->mutex);
    cache_ptr->read_cnt++;

    if (cache_ptr->read_cnt == 1)
        P(&cache_ptr->w);
    
    V(&cache_ptr->mutex);

    /* Critical section, reading happens */

    obj_t *obj_ptr = cache_ptr->first_obj_ptr;

    while (obj_ptr)
    {
        if (is_rl_equal(&obj_ptr->rl, rl_ptr))
            break;
        
        obj_ptr = obj_ptr->next;
    }

    /* Critical section, reading ends */

    P(&cache_ptr->mutex);
    cache_ptr->read_cnt--;

    if (cache_ptr->read_cnt == 0)
        V(&cache_ptr->w);
    
    V(&cache_ptr->mutex);

    return obj_ptr;
}

/*
 * cache_move_first belongs to writer function
 *
 * Implement first readers-writers problem
 * 
 * obj_ptr must be in the doubly linked list of cache 
 */
void cache_move_first(cache_t *cache_ptr, obj_t *obj_ptr)
{
    P(&cache_ptr->w);

    /* Critical section, writing happens */

    /* obj_ptr is already the first object
     * Also ensure cache_ptr->first_obj_ptr is not NULL */
    if (!obj_ptr->pre)
        return;
    
    /* Handle obj_ptr->pre and obj_ptr->next */
    obj_ptr->pre->next = obj_ptr->next;

    if (obj_ptr->next)
        obj_ptr->next->pre = obj_ptr->pre;
    
    else
        /* If obj_ptr is the last object, after it moves first, the previous of obj_ptr should be pointed by cache_ptr->last_obj_ptr */
        cache_ptr->last_obj_ptr = obj_ptr->pre;
    
    /* Handle obj_ptr and cache_ptr->first_obj_ptr */
    obj_ptr->next = cache_ptr->first_obj_ptr;
    cache_ptr->first_obj_ptr->pre = obj_ptr;

    /* Handle obj_ptr and cache_ptr */
    cache_ptr->first_obj_ptr = obj_ptr;
    obj_ptr->pre = NULL;

    /* Critical section, writing ends */

    V(&cache_ptr->w);
}

/*
 * cache_evict_last
 *  belongs to writer function
 *  evicts obj_ptr from the last object place
 *  Meanwhile, change cache_size by subtracting object size
 *
 *  Implement first readers-writers problem
 * 
 * obj_ptr must be in the doubly linked list of cache 
 */
void cache_evict_last(cache_t *cache_ptr)
{
    P(&cache_ptr->w);

    /* Critical section, writing happens */
    // TO DO: evict obj_ptr
    /* Critical section, writing ends */

    V(&cache_ptr->w);
}

/*
 * cache_insert
 *  belongs to writer function
 *  inserts obj_ptr to the first object place
 *  Meanwhile, change cache_size by adding object size
 *
 * Implement first readers-writers problem
 * 
 * obj_ptr must not be in the doubly linked list of cache 
 */
void cache_insert(cache_t *cache_ptr, obj_t *obj_ptr)
{
    P(&cache_ptr->w);

    /* Critical section, writing happens */

    obj_ptr->next = cache_ptr->first_obj_ptr;

    if (cache_ptr->first_obj_ptr)
    {
        cache_ptr->first_obj_ptr->pre = obj_ptr;

        if (!cache_ptr->first_obj_ptr->next)
            cache_ptr->last_obj_ptr = cache_ptr->first_obj_ptr;
    }

    cache_ptr->first_obj_ptr = obj_ptr;
    obj_ptr->pre = NULL;

    cache_ptr->size += obj_ptr->size;

    /* Critical section, writing ends */

    V(&cache_ptr->w);
}
