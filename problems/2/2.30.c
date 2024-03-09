#include <stdio.h>

int tadd_ok(int x, int y)
{
    int s = x + y;

    if (x > 0 && y > 0 && s <=0)
        return 0;

    if (x < 0 && y < 0 && s >=0)
        return 0;

    return 1; 
}

int main()
{
    int x = 0x71234567;
    int y = 0x01234567;

    printf("Not overflow: %d\n", tadd_ok(x, y));

    return 0;
}