/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int bsize, tmp;
    if (M == 32 && N == 32)
        bsize = 8;

    else if (M == 64 && N == 64)
        bsize = 4;

    for (int ii = 0; ii < N; ii += bsize)
    {
        for (int jj = 0; jj < M; jj += bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                    {
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                    {
                        if (i != j)
                        {
                            tmp = A[i][j];
                            B[j][i] = tmp;
                        }
                    }

                    B[i][i] = A[i][i];
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans_no_tmp_desc[] = "Simple row-wise scan transpose without temporary value";
void trans_no_tmp(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            B[j][i] = A[i][j];
        }
    }    

}

/*
 * Matrix transpose without special handling diagonal blocks
 */
char transpose_no_diagonal_desc[] = "Transpose no diagonal";
void transpose_no_diagonal(int M, int N, int A[N][M], int B[M][N])
{
    int bsize = 8;

    for (int ii = 0; ii < N; ii += bsize)
    {
        for (int jj = 0; jj < M; jj += bsize)
        {
            for (int i = ii; i < ii + bsize; i++)
            {
                for (int j = jj; j < jj + bsize; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}

/*
 * Matrix transpose with special handling diagonal blocks
 */
char transpose_diagonal_desc[] = "Transpose diagonal";
void transpose_diagonal(int M, int N, int A[N][M], int B[M][N])
{
    int bsize = 8;

    for (int ii = 0; ii < N; ii += bsize)
    {
        for (int jj = 0; jj < M; jj += bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }
            }
        }
    }
}

/*
 * Matrix transpose with special block scanning order
 * Analysis is in trace_64_1748
 */
char transpose_block_scan_desc[] = "Transpose block scanning order";
void transpose_block_scan(int M, int N, int A[N][M], int B[M][N])
{
    int bsize = 4;

    for (int jj = 0; jj < M; jj += bsize)
    {
        for (int ii = 0; ii < N; ii += bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + bsize; i++)
                {
                    for (int j = jj; j < jj + bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }
            }
        }
    }
}

/*
 * Matrix transpose with special block scanning order without diagonal handling
 */
char transpose_64_special_order_desc[] = "Transpose with special order specific to 64 * 64";
void transpose_64_special_order(int M, int N, int A[N][M], int B[M][N])
{
    int large_bsize = 8;
    int small_bsize = 4;

    for (int ii = 0; ii < N; ii += large_bsize)
    {
        for (int jj = 0; jj < M; jj += large_bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + small_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii + small_bsize; i < ii + large_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii; i < ii + small_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + large_bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }
            }
        }
    }
}

/*
 * Matrix transpose with special block scanning order without diagonal handling
 */
char transpose_64_special_order_desc_2[] = "Transpose with special order specific to 64 * 64 2";
void transpose_64_special_order_2(int M, int N, int A[N][M], int B[M][N])
{
    int large_bsize = 8;
    int small_bsize = 4;

    for (int ii = 0; ii < N; ii += large_bsize)
    {
        for (int jj = 0; jj < M; jj += large_bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + small_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii; i < ii + small_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii + small_bsize; i < ii + large_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }

            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + large_bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }
            }
        }
    }
}

/*
 * Matrix transpose with special block scanning order with diagonal handling
 */
char transpose_64_diagonal_desc[] = "Transpose with special order specific to 64 * 64 with diagonal handling";
void transpose_64_diagonal(int M, int N, int A[N][M], int B[M][N])
{
    int large_bsize = 8;
    int small_bsize = 4;

    for (int ii = 0; ii < N; ii += large_bsize)
    {
        for (int jj = 0; jj < M; jj += large_bsize)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + small_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii + small_bsize; i < ii + large_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }

                for (int i = ii; i < ii + small_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + small_bsize; i++)
                {
                    for (int j = jj; j < jj + small_bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }

                for (int i = ii + small_bsize; i < ii + large_bsize; i++)
                {
                    for (int j = jj; j < jj + small_bsize; j++)
                    {
                        if (i != j + small_bsize)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i - small_bsize][i] = A[i][i - small_bsize];
                }

                for (int i = ii + small_bsize; i < ii + large_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }

                for (int i = ii; i < ii + small_bsize; i++)
                {
                    for (int j = jj + small_bsize; j < jj + large_bsize; j++)
                    {
                        if (i != j - small_bsize)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i + small_bsize][i] = A[i][i + small_bsize];
                }
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc); 

    /* 32 * 32 optimization */
    // registerTransFunction(trans_no_tmp, trans_no_tmp_desc); 

    // registerTransFunction(transpose_no_diagonal, transpose_no_diagonal_desc);

    // registerTransFunction(transpose_diagonal, transpose_diagonal_desc);

    /* 64 * 64 optimization */
    registerTransFunction(transpose_block_scan, transpose_block_scan_desc);

    // Test the order scan in large block size
    registerTransFunction(transpose_64_special_order, transpose_64_special_order_desc);
    registerTransFunction(transpose_64_special_order_2, transpose_64_special_order_desc_2);

    registerTransFunction(transpose_64_diagonal, transpose_64_diagonal_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

