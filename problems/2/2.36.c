#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int tmult_ok(int x, int y)
{
   int64_t p_64 = (int64_t)x * y; 
   int p = x * y;

//    printf("p_64 = %" PRId64 "\n", p_64);

   return p_64 == p;
}

int main()
{
    int x = 5, y = 10;
    printf("Not overflow: %d\n", tmult_ok(x, y));

    x = 2, y = (1 << 31) - 1;
    printf("x = %d, y = %d, x * y = %d, overflow!: %d\n", x, y, x*y, tmult_ok(x, y));
}