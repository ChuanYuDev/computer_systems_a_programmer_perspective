#include <stdio.h>

int main()
{
    int a = 0;
    void *p  = &a;

    printf("%p\n", p);
    printf("%p\n", p + 1);

    return 0;
}