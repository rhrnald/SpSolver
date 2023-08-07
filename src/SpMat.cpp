#include <pybind11/embed.h>
#include "SpMat.h"
#include <iostream> //remove

namespace py = pybind11;
using namespace std;

SpMat::SpMat(){}

SpMat::SpMat(char const* str) {
    SpMat::ReadFromFile(str);
}

void SpMat::ReadFromFile(char const* str) {
    
    py::scoped_interpreter guard{};
    py::module_ scipy_io = py::module_::import("scipy.io");
    py::object file = scipy_io.attr("loadmat")(str); 
    py::object mat = file["J"]; 
    mat = mat.attr("tocsr")();
    shape=mat.attr("_shape").cast<std::tuple<int,int>>();

    cout << "load mat done " << endl;
    //Todo: Auto conversion between python list and c++ vector?
    csrVal=std::vector<double>();
    csrRowPtr=std::vector<int>();
    csrColInd=std::vector<int>();

    for(auto e: mat.attr("indices")) csrColInd.push_back(e.cast<int>());
    for(auto e: mat.attr("indptr")) csrRowPtr.push_back(e.cast<int>());
    for(auto e: mat.attr("data")) csrVal.push_back(e.cast<double>());
    
    cout << "load data done " << endl;
    n=std::get<0>(shape);
    nnz=csrVal.size();

    cout << "load data done " << endl;

    py::object vec = file["R"]; 
    cout << "load vec done " << endl;
    csrVal=std::vector<double>();
    for(auto d : vec)
      for(auto e: d) csrVal.push_back(e.cast<double>());
    cout << "load vec done " << endl;
    return;
}

/*
print
  cout << "Val: "; for(auto &e : A.csrVal) cout << e << " "; printf("\n");
  cout << "RowPtr: "; for(auto &e : RowPtr) cout << e << " "; printf("\n");
  cout << "ColInd: "; for(auto &e : ColInd) cout << e << " "; printf("\n");
*/