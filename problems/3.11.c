long xor(long xl, long yl, int xi, int yi)
{
    long t1 = xl ^ yl;
    long t2 = xi ^ yi;

    return t1 + t2;
}

long assign1(long x)
{
    x = 0;
    return x;
}

long assign2(long x, long y)
{
    x = 0xFF0F0F0F;
    return x;
}