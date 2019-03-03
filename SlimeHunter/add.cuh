#pragma once

#ifndef ADD_CUH
#define ADD_CUH
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);
__global__ void addKernel(int *, const int *, const int *);
// なんでや！
#endif
