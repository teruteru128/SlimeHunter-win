
#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdint.h>

typedef struct random_t{
  int64_t seed;
} Random;

Random *setSeed(Random*, int64_t);
int32_t next(Random*, int32_t);
int64_t nextLong(Random*);
int32_t nextInt(Random*);
int32_t nextIntWithRange(Random*, int32_t);
__device__ Random *setSeedKernel(Random*, int64_t);
__device__ int32_t nextKernel(Random*, int32_t);
__device__ int64_t nextLongKernel(Random*);
__device__ int32_t nextIntKernel(Random*);
__device__ int32_t nextIntWithRangeKernel(Random*, int32_t);


#ifdef __cplusplus
}
#endif
#endif

