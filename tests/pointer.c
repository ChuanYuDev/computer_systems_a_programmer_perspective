#include <stdio.h>

int main()
{
    int a = 4;
    int *p = &a;
    int *p_next = p + 1;

    void *p_void = &a;
    void *p_void_next = p_void + 1;

    printf("p: %p, p_next: %p\n", (void *)p, (void *)p_next);
    printf("p_void: %p, p_void_next: %p\n", p_void, p_void_next);
    return 0;
}