
#ifndef KERNEL_H
#define KERNEL_H

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

cudaError_t addWithCuda(int *, const int *, const int *, unsigned int);
__global__ void addKernel(int *, const int *, const int *);

#endif
