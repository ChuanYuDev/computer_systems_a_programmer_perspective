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

#define SIZE_8 8
#define SIZE_4 4

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
    int i, j, ii, jj, t0, t1, t2, t3;

    if (M == 32 && N == 32)
    {
        for (ii = 0; ii < N; ii += SIZE_8)
        {
            for (jj = 0; jj < M; jj += SIZE_8)
            {
                if (ii != jj)
                {
                    for (i = ii; i < ii + SIZE_8; i++)
                    {
                        for (j = jj; j < jj + SIZE_8; j++)
                            B[j][i] = A[i][j];
                    }
                }
                /* Special handling diagonal blocks */
                else
                {
                    for (i = ii; i < ii + SIZE_8; i++)
                    {
                        for (j = jj; j < jj + SIZE_8; j++)
                        {
                            if (i != j)
                                B[j][i]= A[i][j];
                        }

                        B[i][i] = A[i][i];
                    }
                }
            }
        }
    }
    else if (M == 64 && N == 64)
    {

        for (ii = 0; ii < N; ii += SIZE_8)
        {
            for (jj = 0; jj < M; jj += SIZE_8)
            {
                if (ii != jj)
                {
                    for (i = ii; i < ii + SIZE_4; i++)
                    {
                        for (j = jj; j < jj + SIZE_4; j++)
                            B[j][i] = A[i][j];
                    }
    
                    for (i = ii; i < ii + SIZE_4; i++)
                    {
                        for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                            B[j - SIZE_4][i + SIZE_4] = A[i][j];
                    }
    
                    for (j = 0; j < SIZE_4; j++)
                    {
                        t0 = B[jj + j][ii + SIZE_4];
                        t1 = B[jj + j][ii + SIZE_4 + 1];
                        t2 = B[jj + j][ii + SIZE_4 + 2];
                        t3 = B[jj + j][ii + SIZE_4 + 3];
    
                        for (i = SIZE_4; i < SIZE_8; i++)
                            B[jj + j][ii + i] = A[ii + i][jj + j];
    
                        B[jj + j + SIZE_4][ii] = t0;
                        B[jj + j + SIZE_4][ii + 1] = t1;
                        B[jj + j + SIZE_4][ii + 2] = t2;
                        B[jj + j + SIZE_4][ii + 3] = t3;
                    }
    
                    for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                    {
                        for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                            B[j][i] = A[i][j];
                    }
                }
                else
                {
                    for (t1 = 0; t1 < SIZE_8; t1 += SIZE_4)
                    {
                        for (t0 = 0; t0 < SIZE_8; t0 += SIZE_4)
                        {
                            for (i = ii + t0; i < ii + t0 + SIZE_4; i++)
                            {
                                for (j = jj + t1; j < jj + t1 + SIZE_4; j++)
                                {
                                    if (i + (t1 - t0) != j)
                                        B[j][i] = A[i][j];
                                }

                                B[i + t1 - t0][i] = A[i][i + t1 - t0];
                            }
                        }
                    }
                }
            }
        }
    }

    else if (M == 61 && N == 67)
    {
        for (ii = 0; ii < 64; ii += SIZE_8)
        {
            for (jj = 0; jj < 56; jj += SIZE_8)
            {
                for (i = ii; i < ii + SIZE_8; i++)
                {
                    for (j = jj; j < jj + SIZE_4; j++)
                        B[j][i] = A[i][j];
                }

                for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }

                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }
            }

            for (i = ii; i < ii + SIZE_8; i++)
            {
                for (j = 56; j < 61; j++)
                    B[j][i] = A[i][j];
            }
        }

        for (jj = 0; jj < 61; jj += SIZE_4)
        {
            for (i = 64; i < 67; i++)
            {
                for (j = jj; (j < jj + SIZE_4) && (j < 61); j++)
                    B[j][i] = A[i][j];
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
    for (int ii = 0; ii < N; ii += SIZE_8)
    {
        for (int jj = 0; jj < M; jj += SIZE_8)
        {
            for (int i = ii; i < ii + SIZE_8; i++)
            {
                for (int j = jj; j < jj + SIZE_8; j++)
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
    for (int ii = 0; ii < N; ii += SIZE_8)
    {
        for (int jj = 0; jj < M; jj += SIZE_8)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + SIZE_8; i++)
                {
                    for (int j = jj; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + SIZE_8; i++)
                {
                    for (int j = jj; j < jj + SIZE_8; j++)
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
    for (int jj = 0; jj < M; jj += SIZE_4)
    {
        for (int ii = 0; ii < N; ii += SIZE_4)
        {
            if (ii != jj)
            {
                for (int i = ii; i < ii + SIZE_4; i++)
                {
                    for (int j = jj; j < jj + SIZE_4; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; i < ii + SIZE_4; i++)
                {
                    for (int j = jj; j < jj + SIZE_4; j++)
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
    int i, j;

    for (int ii = 0; ii < N; ii += SIZE_8)
    {
        for (int jj = 0; jj < M; jj += SIZE_8)
        {
            if (ii != jj)
            {
                for (i = ii; i < ii + SIZE_8; i++)
                {
                    for (j = jj; j < jj + SIZE_4; j++)
                        B[j][i] = A[i][j];
                }

                for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }

                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj; j < jj + SIZE_4; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }

                for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                {
                    for (j = jj; j < jj + SIZE_4; j++)
                    {
                        if (i != j + SIZE_4)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i - SIZE_4][i] = A[i][i - SIZE_4];
                }

                for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                    {
                        if (i != j)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i][i] = A[i][i];
                }

                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                    {
                        if (i != j - SIZE_4)
                            B[j][i] = A[i][j];
                    }

                    /* Special handling diagonal elements */
                    B[i + SIZE_4][i] = A[i][i + SIZE_4];
                }
            }
        }
    }
}

char transpose_64_desc[] = "Transpose 64 * 64";
void transpose_64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj, t0, t1, t2, t3;

    for (ii = 0; ii < N; ii += SIZE_8)
    {
        for (jj = 0; jj < M; jj += SIZE_8)
        {
            if (ii != jj)
            {
                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj; j < jj + SIZE_4; j++)
                        B[j][i] = A[i][j];
                }

                for (i = ii; i < ii + SIZE_4; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j - SIZE_4][i + SIZE_4] = A[i][j];
                }

                for (j = 0; j < SIZE_4; j++)
                {
                    t0 = B[jj + j][ii + SIZE_4];
                    t1 = B[jj + j][ii + SIZE_4 + 1];
                    t2 = B[jj + j][ii + SIZE_4 + 2];
                    t3 = B[jj + j][ii + SIZE_4 + 3];

                    for (i = SIZE_4; i < SIZE_8; i++)
                        B[jj + j][ii + i] = A[ii + i][jj + j];

                    B[jj + j + SIZE_4][ii] = t0;
                    B[jj + j + SIZE_4][ii + 1] = t1;
                    B[jj + j + SIZE_4][ii + 2] = t2;
                    B[jj + j + SIZE_4][ii + 3] = t3;
                }

                for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
                {
                    for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                        B[j][i] = A[i][j];
                }
            }
            else
            {
                for (t1 = 0; t1 < SIZE_8; t1 += SIZE_4)
                {
                    for (t0 = 0; t0 < SIZE_8; t0 += SIZE_4)
                    {
                        for (i = ii + t0; i < ii + t0 + SIZE_4; i++)
                        {
                            for (j = jj + t1; j < jj + t1 + SIZE_4; j++)
                            {
                                if (i + (t1 - t0) != j)
                                    B[j][i] = A[i][j];
                            }

                            B[i + t1 - t0][i] = A[i][i + t1 - t0];
                        }
                    }
                }
            }
        }
    }
}

char transpose_61_67_desc[] = "Transpose 61 * 67 matrix";
void transpose_61_67(int M, int N, int A[N][M], int B[M][N])
{
    for (int jj = 0; jj < M; jj += SIZE_4)
    {
        for (int ii = 0; ii < N; ii += SIZE_4)
        {
            if (ii != jj)
            {
                for (int i = ii; (i < ii + SIZE_4) && (i < N); i++)
                {
                    for (int j = jj; (j < jj + SIZE_4) && (j < M); j++)
                        B[j][i] = A[i][j];
                }
            }
            /* Special handling diagonal blocks */
            else
            {
                for (int i = ii; (i < ii + SIZE_4) && (i < N); i++)
                {
                    for (int j = jj; (j < jj + SIZE_4) && (j < M); j++)
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

char transpose_61_67_2_desc[] = "Transpose 61 * 67 matrix 2";
void transpose_61_67_2(int M, int N, int A[N][M], int B[M][N])
{
    for (int jj = 0; jj < M; jj += SIZE_4)
    {
        for (int ii = 0; ii < N; ii += SIZE_4)
        {
            for (int i = ii; (i < ii + SIZE_4) && (i < N); i++)
            {
                for (int j = jj; (j < jj + SIZE_4) && (j < M); j++)
                    B[j][i] = A[i][j];
            }
        }
    }
}

char transpose_61_67_2_2_desc[] = "Transpose 61 * 67 matrix 2.2";
void transpose_61_67_2_2(int M, int N, int A[N][M], int B[M][N])
{
    for (int ii = 0; ii < N; ii += SIZE_4)
    {
        for (int jj = 0; jj < M; jj += SIZE_4)
        {
            for (int i = ii; (i < ii + SIZE_4) && (i < N); i++)
            {
                for (int j = jj; (j < jj + SIZE_4) && (j < M); j++)
                    B[j][i] = A[i][j];
            }
        }
    }
}

char transpose_61_67_3_desc[] = "Transpose 61 * 67 matrix 3";
void transpose_61_67_3(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj;

    for (ii = 0; ii < 64; ii += SIZE_8)
    {
        for (jj = 0; jj < 56; jj += SIZE_8)
        {
            for (i = ii; i < ii + SIZE_8; i++)
            {
                for (j = jj; j < jj + SIZE_4; j++)
                    B[j][i] = A[i][j];
            }

            for (i = ii + SIZE_4; i < ii + SIZE_8; i++)
            {
                for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                    B[j][i] = A[i][j];
            }

            for (i = ii; i < ii + SIZE_4; i++)
            {
                for (j = jj + SIZE_4; j < jj + SIZE_8; j++)
                    B[j][i] = A[i][j];
            }
        }

        for (i = ii; i < ii + SIZE_8; i++)
        {
            for (j = 56; j < 61; j++)
                B[j][i] = A[i][j];
        }
    }

    for (jj = 0; jj < 61; jj += SIZE_4)
    {
        for (i = 64; i < 67; i++)
        {
            for (j = jj; (j < jj + SIZE_4) && (j < 61); j++)
                B[j][i] = A[i][j];
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
    // registerTransFunction(transpose_block_scan, transpose_block_scan_desc);
    // registerTransFunction(transpose_64_diagonal, transpose_64_diagonal_desc);
    // registerTransFunction(transpose_64, transpose_64_desc);

    /* 61 * 67 optimization*/
    // registerTransFunction(transpose_61_67, transpose_61_67_desc);
    // registerTransFunction(transpose_61_67_2, transpose_61_67_2_desc);
    // registerTransFunction(transpose_61_67_2_2, transpose_61_67_2_2_desc);
    // registerTransFunction(transpose_61_67_3, transpose_61_67_3_desc);
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

