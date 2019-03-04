
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "mcSlimeChunkOracle.cuh"

#if 1
__device__ SlimeChunkSeed* setMCSeedKernel(SlimeChunkSeed* t, int64_t seed) {
	t->seed = seed;
	return t;
}

__device__ int64_t getMCSeedKernel(SlimeChunkSeed* seed, int32_t chunkX, int32_t chunkZ) {
	return seed->seed + chunkX * chunkX * 0x4c1906 + chunkX * 0x5ac0db + chunkZ * chunkZ * 0x4307a7L + chunkZ * 0x5f24f ^ 0x3ad8025f;
}

__device__ static bool isSlimeChunkKernel(Random* rnd) {
	return nextIntWithRangeKernel(rnd, 10) == 0;
}

__device__ bool isSlimeChunkXZKernel(SlimeChunkSeed* seed, int64_t chunkX, int64_t chunkZ) {
	return isSlimeChunkKernel(setSeedKernel(&seed->rnd, getMCSeedKernel(seed, (int32_t)chunkX, (int32_t)chunkZ)));
}
#else

__device__ SlimeChunkSeed* setMCSeedKernel(SlimeChunkSeed* t, int64_t seed) {
	t->seed = seed;
	return t;
}

__device__ int64_t getMCSeedKernel(SlimeChunkSeed* seed, int32_t chunkX, int32_t chunkZ) {
	return seed->seed + chunkX * chunkX * 0x4c1906 + chunkX * 0x5ac0db + chunkZ * chunkZ * 0x4307a7L + chunkZ * 0x5f24f ^ 0x3ad8025f;
}

__device__ bool isSlimeChunkKernel(Random* rnd) {
	//return nextIntWithRangeKernel(rnd, 10) == 0;
	return false;
}

__device__ bool isSlimeChunkXZKernel(SlimeChunkSeed* mCSeed, int64_t chunkX, int64_t chunkZ) {
	//int64_t seed = getMCSeedKernel(mCSeed, (int32_t)chunkX, (int32_t)chunkZ);
	//Random *rnd = setSeedKernel(&mCSeed->rnd, seed);
	//return isSlimeChunkKernel(rnd);
	isSlimeChunkKernel(&mCSeed->rnd);
	return 0;
}
#endif
