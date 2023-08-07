cmake_minimum_required (VERSION 3.5)

enable_language(C CXX CUDA)

set(CMAKE_CXX_COMPILER icpx)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsycl -std=c++17")

find_package(pybind11 CONFIG)
find_package(CUDA)

# Set MKL_ROOT directory
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(MKL)

include_directories(include ${pybind11_INCLUDE_DIRS})


set(MKL_LIBRARIES mkl_sycl ${MKL_LIBRARIES})
message(!!MKL LIBRAREIS: ${MKL_LIBRARIES})

#cuda_add_library(SpSolver SHARED src/CudaSolver.cpp src/MKLSolver.cpp src/SpMat.cpp )
cuda_add_executable(main src/main.cpp src/CudaSolver.cpp src/MKLSolver.cpp src/SpMat.cpp )
target_link_libraries(main pybind11::embed ${CUDA_cusolver_LIBRARY} ${CUDA_cusparse_LIBRARY} ${MKL_LIBRARIES})
