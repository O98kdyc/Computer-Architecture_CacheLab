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
    int i,j,i1,j1;
    int tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
if(M==61)
{
    for(i=0;i<N;i+=16)
        for(j=0;j<M;j+=16)
        {
            for(i1=i;i1<i+16&&i1<N;i1++)
                for(j1=j;j1<j+16&&j1<M;j1++)
                    B[j1][i1]=A[i1][j1];
        }
}
if(M==32)
{
   for(i=0;i<N;i+=8)
      for(j=0;j<M;j+=8)
   {
     if(i==j)
    {
      for(i1=i;i1<i+8 && i1<N;i1++)
     {
       tmp0=A[i1][j];
       tmp1=A[i1][j+1];
       tmp2=A[i1][j+2];
       tmp3=A[i1][j+3];
       tmp4=A[i1][j+4];
       tmp5=A[i1][j+5];
       tmp6=A[i1][j+6];
       tmp7=A[i1][j+7];
       B[j][i1]=tmp0;
       B[j+1][i1]=tmp1;
       B[j+2][i1]=tmp2;
       B[j+3][i1]=tmp3;
       B[j+4][i1]=tmp4;
       B[j+5][i1]=tmp5;
       B[j+6][i1]=tmp6;
       B[j+7][i1]=tmp7;
     }
    }
     else
      for(i1=i;i1<i+8 && i1<N;i1++)
        for(j1=j;j1<j+8 && j1<M;j1++)
           B[j1][i1]=A[i1][j1];
   }
}
if(M==64)
{
for(i=0;i<N;i+=8)
for(j=0;j<M;j+=8)
{
   for(i1=i;i1<i+4 && i1<N;i1++)
     {
       tmp0=A[i1][j];
       tmp1=A[i1][j+1];
       tmp2=A[i1][j+2];
       tmp3=A[i1][j+3];
       tmp4=A[i1][j+4];
       tmp5=A[i1][j+5];
       tmp6=A[i1][j+6];
       tmp7=A[i1][j+7];
       B[j][i1]=tmp0;
       B[j+1][i1]=tmp1;
       B[j+2][i1]=tmp2;
       B[j+3][i1]=tmp3;
       B[j][i1+4]=tmp4;
       B[j+1][i1+4]=tmp5;
       B[j+2][i1+4]=tmp6;
       B[j+3][i1+4]=tmp7;
     }
    for(j1=j;j1<j+4&&j1<M;j1++)
{
    tmp0=A[i+4][j1];
    tmp1=A[i+5][j1];
    tmp2=A[i+6][j1];
    tmp3=A[i+7][j1];

    i1=A[i+j1+4-j][j1+4];

    tmp4=B[j1][i+4];
    tmp5=B[j1][i+5];
    tmp6=B[j1][i+6];
    tmp7=B[j1][i+7];

    B[j1][i+4]=tmp0;
    B[j1][i+5]=tmp1;
    B[j1][i+6]=tmp2;
    B[j1][i+7]=tmp3;
    
    B[j1+4][i]=tmp4;
    B[j1+4][i+1]=tmp5;
    B[j1+4][i+2]=tmp6;
    B[j1+4][i+3]=tmp7;

    if(j1+4-j!=4) B[j1+4][i+4]=A[i+4][j1+4];
    if(j1+4-j!=5) B[j1+4][i+5]=A[i+5][j1+4];   
    if(j1+4-j!=6) B[j1+4][i+6]=A[i+6][j1+4];
    if(j1+4-j!=7) B[j1+4][i+7]=A[i+7][j1+4];

    B[j1+4][i+j1+4-j]=i1;

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
    registerTransFunction(trans, trans_desc); 

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

