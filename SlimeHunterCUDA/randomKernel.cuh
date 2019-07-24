#pragma once

#ifndef RANDOM_KERNEL_CUH
#define RANDOM_KERNEL_CUH

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdint.h>
#include "jrandom.cuh"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
	cudaError_t randomWithCuda(int64_t * out, int64_t *in, Random* random, size_t n);
	__global__ void genRandomKernel(int64_t *out, const int64_t *in, Random*, const size_t n);
#endif
