

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdint.h>
#include "jrandom.cuh"

#ifdef __cplusplus
extern "C" {
#endif

	cudaError_t randomWithCuda(int64_t *, int64_t *, Random *, size_t);
	__global__ void genRandomKernel(int64_t *out, const int64_t *in, Random*, const size_t n);

#ifdef __cplusplus
}
#endif
