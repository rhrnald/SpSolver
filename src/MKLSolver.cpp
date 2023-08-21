#include "MKLSolver.h"
//#include "oneapi/mkl.hpp"
#include "mkl.h"

//using namespace oneapi;

//Todo MKL Error handling
void set_iparm(MKL_INT *iparm) {
  iparm[34]=1; // zero-base
  iparm[55]=1; // pivot and calculation of diagonal arrays turned on
}

int mklPardiso(SpMat &A, double* b, double* x) {
  //pardiso(_MKL_DSS_HANDLE_t pt, const MKL_INT *maxfct, const MKL_INT *mnum, const MKL_INT *mtype, const MKL_INT *phase, const MKL_INT *n, const void *a, const MKL_INT *ia, const MKL_INT *ja, MKL_INT *perm, const MKL_INT *nrhs, MKL_INT *iparm, const MKL_INT *msglvl, void *b, void *x, MKL_INT *error);
  void *pt[64];
  MKL_INT maxfct=1, mnum=1, mtype=11, n = A.n, perm=1, nrhs=1, msglvl=0, error=0;
  MKL_INT iparm[64];
  for(int i=0; i<64; i++) pt[i]=0, iparm[i]=0;
  printf("START!!!\n"); fflush(stdout);
  //pardisoinit(pt, &mtype, iparm);
  set_iparm(iparm);

  std::vector<MKL_INT> RowPtr(A.csrRowPtr.size());
  std::vector<MKL_INT> ColInd(A.csrColInd.size());
  for(int i=0; i<RowPtr.size(); i++) RowPtr[i] = A.csrRowPtr[i]; //Todo int vector->long long vector casting?
  for(int i=0; i<ColInd.size(); i++) ColInd[i] = A.csrColInd[i];
  
  MKL_INT phase=13;
  //pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n, A.csrVal.data(), RowPtr.data(), ColInd.data(), &perm, &nrhs, iparm, &msglvl, b, x, &error);

  double *df = (double*)malloc(sizeof(double)*n);
  double *da = (double*)malloc(sizeof(double)*n);

  //pardiso_getdiag(pt, df, da, &mnum, &error);
  printf("Error: %lld\n", error);
  printf("DA: ");for(int i=0; i<std::min(10,(int)n); i++) printf("%lf ", da[i]); printf("\n");
  printf("DF: ");for(int i=0; i<std::min(10,(int)n); i++) printf("%lf ", df[i]); printf("\n");

  double m = 100;
  for(int i=0; i<n; i++) m=std::min(m, abs(df[i]));
  int c5=0, c10=0, c15=0;
  for(int i=0; i<n; i++) if(abs(df[i])<1e-5) c5++;
  for(int i=0; i<n; i++) if(abs(df[i])<1e-10) c10++;
  for(int i=0; i<n; i++) if(abs(df[i])<1e-15) c15++;
  printf("min: %lf\n", m);
  printf("n: %lld\n", n);
  printf("count under 1e-5: %d\n", c5);
  printf("count under 1e-10: %d\n", c10);
  printf("count under 1e-15: %d\n", c15);
  return (int)error;
}

int myPardiso(SpMat &A, double* b, double* x) {
  //mkl::sparse::matrix_handle_t handle;
  //mkl::sparse::init_matrix_handle (&handle);
}
