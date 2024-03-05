#include <stdio.h>

int main()
{
    int val = 0xFEDCBA98;
    printf("%x\n", val);

    int lval = 0xFEDCBA98 << 32;
    int aval = 0xFEDCBA98 >> 36;
    int uval = 0xFEDCBA98 >> 40;

    printf("%x, %x, %x\n", lval, aval, uval);
}