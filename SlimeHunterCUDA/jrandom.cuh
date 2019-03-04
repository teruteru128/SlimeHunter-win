
#ifndef JRANDOM_CUH
#define JRANDOM_CUH

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "internal_jrandom.h"

	__device__ Random* setSeedKernel(Random*, int64_t);
	__device__ int32_t nextKernel(Random*, int32_t);
	__device__ int64_t nextLongKernel(Random*);
	__device__ int32_t nextIntKernel(Random*);
	__device__ int32_t nextIntWithRangeKernel(Random*, int32_t);


#endif
