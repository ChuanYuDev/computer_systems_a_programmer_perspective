#include <stdio.h>

int main()
{
    {
        int a[] = {1, 10, 20};
        int *p = a;

        printf("Increment and derefence: %d\n", *p++);
        printf("%d\n", *p);
    }

    {
        printf("Shift and add: %d\n", 1 << 2 + 3 << 4);
    }
}