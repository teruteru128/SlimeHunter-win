
#ifndef MC_SLIME_CHUNK_ORACLE_H
#define MC_SLIME_CHUNK_ORACLE_H

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "jrandom.cuh"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct SlimeChunkSeed_t{
  int64_t seed;
  Random rnd;
} SlimeChunkSeed;

int64_t getMCSeed(SlimeChunkSeed*, int32_t, int32_t);
void setMCSeed(SlimeChunkSeed*, int64_t);
bool isSlimeChunkXZ(SlimeChunkSeed *, int64_t, int64_t);
bool isSlimeChunk(Random *);
__device__ int64_t getMCSeedKernel(SlimeChunkSeed*, int32_t , int32_t);
__device__ void setMCSeedKernel(SlimeChunkSeed*, int64_t);
__device__ bool isSlimeChunkXZKernel(SlimeChunkSeed *, int64_t, int64_t);
__device__ bool isSlimeChunkKernel(Random *);


#ifdef __cplusplus
}
#endif
#endif

