
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

#include "jrandom.cuh"
#include "mcSlimeChunkOracle.cuh"

__host__ __device__ static int64_t initialScrambleKernel(uint64_t seed) {
	return (seed ^ MULTIPLIER) & MASK;
}

__host__ __device__ Random* setSeedKernel(Random *rnd, int64_t seed) {
	rnd->seed = initialScrambleKernel(seed);
	return rnd;
}

__host__ __device__ int32_t nextKernel(Random* rnd, int32_t bits) {
	uint64_t oldseed, nextseed;
	uint64_t seed = rnd->seed;
	oldseed = seed;
	nextseed = (oldseed * MULTIPLIER + ADDEND) & MASK;
	rnd->seed = nextseed;
	return (uint32_t)(nextseed >> (48 - bits));
}

__host__ __device__ int64_t nextLongKernel(Random* rnd) {
	return ((int64_t)nextKernel(rnd, 32) << 32) + nextKernel(rnd, 32);
}

__host__ __device__ int32_t nextIntKernel(Random* rnd) {
	return nextKernel(rnd, 32);
}

__host__ __device__ int32_t nextIntWithRangeKernel(Random *rnd, int32_t bound) {
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

__global__ void genRandomKernel(int64_t *out, const int64_t *in, Random *random, const size_t n) {
	unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i >= n) {
		return;
	}
	out[i] = nextLongKernel(setSeedKernel(&random[i], in[i]));
	//out[i] = in[i];
}

cudaError_t randomWithCuda(int64_t * out, int64_t *in, Random* random, size_t n) {
	int64_t *dev_in = NULL;
	int64_t *dev_out = NULL;
	Random *dev_random = NULL;
	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed\n");
		goto error;
	}

	cudaStatus = cudaMalloc(&dev_out, n * sizeof(int64_t));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed(dev_out)\n");
		goto error;
	}

	cudaStatus = cudaMalloc(&dev_in, n * sizeof(int64_t));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed(dev_in)\n");
		goto error;
	}

	cudaStatus = cudaMalloc(&dev_random, n * sizeof(Random));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed(dev_in)\n");
		goto error;
	}

	cudaStatus = cudaMemcpy(dev_in, in, n * sizeof(int64_t), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed(dev_in)\n");
		goto error;
	}

	cudaStatus = cudaMemcpy(dev_random, random, n * sizeof(Random), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed(dev_in)\n");
		goto error;
	}

	genRandomKernel << <1, n >> > (dev_out, dev_in, dev_random, n);

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(out, dev_out, n * sizeof(int64_t), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto error;
	}
error:
	cudaFree(dev_in);
	cudaFree(dev_out);
	cudaFree(dev_random);
	return cudaStatus;
}
