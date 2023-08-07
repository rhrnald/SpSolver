#include <iostream>
#include <ctime>
#include "CudaSolver.h"
#include "MKLSolver.h"

using namespace std;


int main(int argc, char **argv){
    SpMat A("/home/n5/chaewon/sparse/solvertest1.mat");
    printf("%d\n", A.n); fflush(stdout);
    double* b = A.bVal.data();
    double *x = (double*)malloc(sizeof(double)*A.n);

    //printf("Cu Error: %d\n", cuSolverSpSolver(A, b, x));
    //printf("Cu Result: "); for(int i=0; i<min(A.n, 10); i++) printf("%lf ", x[i]); printf("\n");

    
    clock_t start, finish;
    double duration;
 
    start = clock();
    printf("MKL Error: %d\n", mklPardiso(A, b, x));
    finish = clock();
 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << duration << "sec" << endl;

    printf("MKL Result: "); for(int i=0; i<min(A.n, 10); i++) printf("%.3lf ", x[i]); printf("\n");
    //Todo Check Ax-b
}

