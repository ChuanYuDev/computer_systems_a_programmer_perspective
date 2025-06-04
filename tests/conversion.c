#include <stdio.h>

int main()
{
    int x = -1;
    unsigned u = 2147483648;

    printf("x = %u = %d\n", x, x);
    printf("u = %u = %d\n", u, u);

    x = -3;
    int y = 4; 
    printf("x mod y = %d\n", x % y);

    int w = 2147483647;
    printf("(w + 1) mod 32 = %d\n", (w + 1) % 32);
    printf("(w + 2) mod 32 = %d\n", (w + 2) % 32);

    return 0;
}