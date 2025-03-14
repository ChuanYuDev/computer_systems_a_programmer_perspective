#include <stdio.h>

static int A[3][5];

int main()
{
    printf("A: %p\n", A);
    printf("A[1]: %p\n", A[1]);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            A[i][j] = i + j;
        }
    }

    printf("A: %p\n", A);
    printf("A[1]: %p\n", A[1]);

    return 0;
}