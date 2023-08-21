#include "slu_ddefs.h"
#include <iostream>
#include <ctime>
#include "CudaSolver.h"
#include "MKLSolver.h"

using namespace std;


void f(double *a, int *asub, int *xa, int n, int m, int nnz)
{
    /*
     * Purpose
     * =======
     *
     * This is the small 5x5 example used in the Sections 2 and 3 of the
     * Users’ Guide to illustrate how to call a SuperLU routine, and the
     * matrix data structures used by SuperLU.
     *
     */
    SuperMatrix A, L, U, B;
    double *rhs;
    int *perm_r; /* row permutations from partial pivoting */
    int *perm_c; /* column permutation vector */
    int nrhs, info, i, permc_spec;
    superlu_options_t options;
    SuperLUStat_t stat;
    printf("START\n"); fflush(stdout);
    /* Initialize matrix A. */
    /*
    m = n = 5;
    nnz = 12;
    if (!(a = doubleMalloc(nnz)))
        ABORT("Malloc fails for a[].");
    if (!(asub = intMalloc(nnz)))
        ABORT("Malloc fails for asub[].");
    if (!(xa = intMalloc(n + 1)))
        ABORT("Malloc fails for xa[].");
    s = 19.0;
    u = 21.0;
    p = 16.0;
    e = 5.0;
    r = 18.0;
    l = 12.0;
    a[0] = s;
    a[1] = l;
    a[2] = l;
    a[3] = u;
    a[4] = l;
    a[5] = l;
    a[6] = u;
    a[7] = p;
    a[8] = u;
    a[9] = e;
    a[10] = u;
    a[11] = r;
    asub[0] = 0;
    asub[1] = 1;
    asub[2] = 4;
    asub[3] = 1;
    asub[4] = 2;
    asub[5] = 4;
    asub[6] = 0;
    asub[7] = 2;
    asub[8] = 0;
    asub[9] = 3;
    asub[10] = 3;
    asub[11] = 4;
    xa[0] = 0;
    xa[1] = 3;
    xa[2] = 6;
    xa[3] = 8;
    xa[4] = 10;
    xa[5] = 12;*/
    /* Create matrix A in the format expected by SuperLU. */
    dCreate_CompCol_Matrix(&A, m, n, nnz, a, asub, xa, SLU_NC, SLU_D, SLU_GE);
    printf("Create compcol matrix done\n"); fflush(stdout);
    /* Create right-hand side matrix B. */
    nrhs = 1;
    if (!(rhs = doubleMalloc(m * nrhs)))
        ABORT("Malloc fails for rhs[].");
    for (i = 0; i < m; ++i)
        rhs[i] = 1.0;
    dCreate_Dense_Matrix(&B, m, nrhs, rhs, m, SLU_DN, SLU_D, SLU_GE);
    printf("Create dense done\n"); fflush(stdout);
    if (!(perm_r = intMalloc(m)))
        ABORT("Malloc fails for perm_r[].");
    if (!(perm_c = intMalloc(n)))
        ABORT("Malloc fails for perm_c[].");
    /* Set the default input options. */
    set_default_options(&options);
    options.ColPerm = NATURAL;
    /* Initialize the statistics variables. */
    StatInit(&stat);
    /* Solve the linear system. */
    printf("Dgssv start\n"); fflush(stdout);
    dgssv(&options, &A, perm_c, perm_r, &L, &U, &B, &stat, &info);
    printf("Dgssv done\n"); fflush(stdout);
    return;
    //dPrint_CompCol_Matrix("A", &A);
    //dPrint_CompCol_Matrix("U", &U);
    //dPrint_SuperNode_Matrix("L", &L);
    //print_int_vec("\nperm_r", m, perm_r);
    /* De-allocate storage */
    SUPERLU_FREE(rhs);
    SUPERLU_FREE(perm_r);
    SUPERLU_FREE(perm_c);
    Destroy_CompCol_Matrix(&A);
    Destroy_SuperMatrix_Store(&B);
    Destroy_SuperNode_Matrix(&L);
    Destroy_CompCol_Matrix(&U);
    StatFree(&stat);
}

int main(int argc, char **argv){
    SpMat A("/home/n5/chaewon/mat/matrix_2037609.mat");
    
    double* b = A.bVal.data();
    double *x = (double*)malloc(sizeof(double)*A.n);
    printf("n: %d\nnnz: %d\nval, ptr, idx, b: %zu %zu %zu %zu\n", A.n, A.nnz, A.csrVal.size(), A.csrRowPtr.size(), A.csrColInd.size(), A.bVal.size()); fflush(stdout);

    //printf("Cu Error: %d\n", cuSolverSpSolver(A, b, x));
    //printf("Cu Result: "); for(int i=0; i<min(A.n, 10); i++) printf("%lf ", x[i]); printf("\n");

    for(int i=0; i<10; i++) printf("%lf %d %d %lf\n", A.csrVal[i], A.csrRowPtr[i], A.csrColInd[i], A.bVal[i]);
    
    clock_t start, finish;
    double duration;
 
    start = clock();
    //printf("Cu Error: %d\n", cuSolverSpSolver(A, b, x));
    /*std::vector<double> RowPtr(A.csrRowPtr.size());
    std::vector<double> ColInd(A.csrColInd.size());
      for(int i=0; i<RowPtr.size(); i++) RowPtr[i] = A.csrRowPtr[i]; //Todo int vector->long long vector casting?
    for(int i=0; i<ColInd.size(); i++) ColInd[i] = A.csrColInd[i];*/

    //f(A.csrVal.data(), A.csrColInd.data(), A.csrRowPtr.data(), A.n, A.n, A.nnz);

    printf("MKL Error: %d\n", mklPardiso(A, b, x));
    finish = clock();
 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << "sec" << endl;

    printf("MKL Result: "); for(int i=0; i<min(A.n, 10); i++) printf("%.3lf ", x[i]); printf("\n");
    //Todo Check Ax-b
}

