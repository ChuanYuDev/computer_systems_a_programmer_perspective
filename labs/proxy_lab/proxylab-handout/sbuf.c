#include "helper.h"
#include "sbuf.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_int(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;
    sp->front = sp->rear = 0;   /* Empty buffer iff front == rear */

    Sem_init(&sp->mutex, 1);
    Sem_init(&sp->slots, n);
    Sem_init(&sp->items, 0);
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp)
{
    free(sp->buf);
}

/* Insert item onto the rear of shared buffer sp */
void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);
    P(&sp->mutex);

    sp->rear = (++sp->rear) % sp->n;
    sp->buf[sp->rear] = item;

    V(&sp->mutex);
    V(&sp->items);
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp)
{
    int item;

    P(&sp->items);
    P(&sp->mutex);

    sp->front = (++sp->front) % sp->n;
    item = sp->buf[sp->front];

    V(&sp->mutex);
    V(&sp->slots);

    return item;
}