#ifndef SPMAT
#define SPMAT
//#include <cusparse_v2.h>
#include <vector>
#include <tuple>

class SpMat {
public:
    //cusparseMatDescr_t descrA;
    std::tuple<int,int> shape;
    int n,nnz;
    std::vector<double> csrVal;
    std::vector<int> csrRowPtr, csrColInd;
    std::vector<double> bVal;

//public:
    SpMat();
    SpMat(char const* str);

    void ReadFromFile(char const* str);
    //print mat
};

typedef struct {
  int *row_offsets;
  int *column_indices;
  float *values;
} CompressedSparseRow;
#endif
