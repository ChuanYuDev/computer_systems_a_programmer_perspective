#include <stdio.h>

void decode1(long *xp, long *yp, long *zp)
{
    long x = *xp;
    long y = *yp;
    long z = *zp;

    *yp = x; 
    *zp = y;
    *xp = z;
}

int main()
{
    long x = 4, y = 10, z = 2;
    decode1(&x, &y, &z);
    printf("x = %ld, y = %ld, z = %ld\n", x, y, z);    
    return 0;
}