
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "jrandom.cuh"

__device__ static int64_t initialScrambleKernel(uint64_t seed) {
	return (seed ^ MULTIPLIER) & MASK;
}

__device__ Random* setSeedKernel(Random *rnd, int64_t seed) {
	rnd->seed = initialScrambleKernel(seed);
	return rnd;
}

__device__ int32_t nextKernel(Random* rnd, int32_t bits) {
	uint64_t oldseed, nextseed;
	uint64_t seed = rnd->seed;
	oldseed = seed;
	nextseed = (oldseed * MULTIPLIER + ADDEND) & MASK;
	rnd->seed = nextseed;
	return (uint32_t)(nextseed >> (48 - bits));
}

__device__ int64_t nextLongKernel(Random* rnd) {
	return ((int64_t)(nextKernel(rnd, 32)) << 32) + nextKernel(rnd, 32);
}

__device__ int32_t nextIntKernel(Random* rnd) {
	return nextKernel(rnd, 32);
}

__device__ int32_t nextIntWithRangeKernel(Random *rnd, int32_t bound) {
	if (bound <= 0) {
		// err!
		return 0;
	}
	int32_t r = nextKernel(rnd, 31);
	int32_t m = bound - 1;
	if ((bound & m) == 0) {
		r = (int32_t)((bound * (int64_t)r) >> 31);
	}
	else {
		int32_t u;
		for (u = r; u - (r = u % bound) + m < 0; u = nextKernel(rnd, 31));
	}
	return r;
}
