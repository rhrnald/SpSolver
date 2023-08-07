/*#include "CudaSolver.h"
#include <cusparse_v2.h>
#include <cusolverSp.h>

//TODO: cuda error handling
int cuSolverSpSolver(SpMat &A, double* b, double* x) {
  cusolverSpHandle_t handle; cusolverSpCreate(&handle);
  
  cusparseMatDescr_t descrA; cusparseCreateMatDescr(&descrA);
  cusparseSetMatType     (descrA, CUSPARSE_MATRIX_TYPE_GENERAL);
  cusparseSetMatIndexBase(descrA, CUSPARSE_INDEX_BASE_ZERO);  

  int reorder=0;
  double tol=0;
  int singularity=0;

  cusolverStatus_t t = cusolverSpDcsrlsvluHost(handle,A.n,A.nnz,descrA,A.csrVal.data(),A.csrRowPtr.data(),A.csrColInd.data(),b,tol,reorder,x,&singularity);
  printf("singularity: %d\n", singularity);

  return t;
}
*/