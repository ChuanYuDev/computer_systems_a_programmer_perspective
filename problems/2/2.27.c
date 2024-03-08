#include <stdio.h>

int uadd_ok(unsigned x, unsigned y)
{
    unsigned s = x + y;
    return s >= x;
}

int main()
{
    unsigned x = 0xABCDEF12;
    unsigned y = 0x1BCDEF34;

    printf("Not overflow: %d\n", uadd_ok(x, y));

    return 0;
}