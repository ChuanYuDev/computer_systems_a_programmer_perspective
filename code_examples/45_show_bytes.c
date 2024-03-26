#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
        printf("%p\t0x%.2x\n", start + i, start[i]);
    
    printf("\n");
}

int main()
{
    int a = 15213;
    printf("int a = %d\n", a);
    show_bytes((byte_pointer) &a, sizeof(int));
    return 0;
}