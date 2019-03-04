#pragma once
#ifndef MC_SLIME_CHUNK_ORACLE_CUH
#define MC_SLIME_CHUNK_ORACLE_CUH

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "jrandom.cuh"
#include "commonMcSlimeChunkOracle.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
	__device__ int64_t getMCSeedKernel(SlimeChunkSeed*, int32_t, int32_t);
	__device__ SlimeChunkSeed* setMCSeedKernel(SlimeChunkSeed*, int64_t);
	__device__ bool isSlimeChunkXZKernel(SlimeChunkSeed *, int64_t, int64_t);
#endif

