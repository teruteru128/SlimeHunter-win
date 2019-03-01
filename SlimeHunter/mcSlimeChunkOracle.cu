
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>
#include "mcSlimeChunkOracle.cuh"

void setMCSeed(SlimeChunkSeed* t, int64_t seed) {
	t->seed = seed;
}

int64_t getMCSeed(SlimeChunkSeed* seed, int32_t chunkX, int32_t chunkZ) {
	return seed->seed + chunkX * chunkX * 0x4c1906 + chunkX * 0x5ac0db + chunkZ * chunkZ * 0x4307a7L + chunkZ * 0x5f24f ^ 0x3ad8025f;
}

bool isSlimeChunk(Random* rnd) {
	return nextIntWithRange(rnd, 10) == 0;
}

bool isSlimeChunkXZ(SlimeChunkSeed* seed, int64_t chunkX, int64_t chunkZ) {
	Random* rnd = &seed->rnd;
	setSeed(rnd, getMCSeed(seed, (int32_t)chunkX, (int32_t)chunkZ));
	return isSlimeChunk(rnd);
}
/**/
__device__ void setMCSeedKernel(SlimeChunkSeed* t, int64_t seed) {
	t->seed = seed;
}

__device__ int64_t getMCSeedKernel(SlimeChunkSeed* seed, int32_t chunkX, int32_t chunkZ) {
	return seed->seed + chunkX * chunkX * 0x4c1906 + chunkX * 0x5ac0db + chunkZ * chunkZ * 0x4307a7L + chunkZ * 0x5f24f ^ 0x3ad8025f;
}

__device__ bool isSlimeChunkKernel(Random* rnd) {
	return nextIntWithRangeKernel(rnd, 10) == 0;
}

__device__ bool isSlimeChunkXZKernel(SlimeChunkSeed* seed, int64_t chunkX, int64_t chunkZ) {
	Random* rnd = &seed->rnd;
	setSeedKernel(rnd, getMCSeedKernel(seed, (int32_t)chunkX, (int32_t)chunkZ));
	return isSlimeChunkKernel(rnd);
}

