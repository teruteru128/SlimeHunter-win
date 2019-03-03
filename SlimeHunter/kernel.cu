
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <windows.h>
#include <time.h>
#include <random>
#include "jrandom.cuh"
#include "add.cuh"
#include "randomKernel.cuh"


int main(int argc, char* argv[])
{
	int arraySize = 5;
	const int a[5] = {
		1, 2, 3, 4, 5
	};
	const int b[5] = {
		10, 20, 30, 40, 50
	};
	int c[5] = { 0 };

	// Add vectors in parallel.
	cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}
	printf("{1, 2, 3, 4, 5} +{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100} = {%d,%d,%d,%d,%d}\n",
		c[0], c[1], c[2], c[3], c[4]);
	
	size_t n = 334;
	int64_t *in = NULL;
	int64_t *out = NULL;
	Random *random = NULL;

	in = (int64_t *)calloc(n, sizeof(size_t));
	out = (int64_t *)calloc(n, sizeof(size_t));
	random = (Random *)calloc(n, sizeof(Random));

	std::random_device rnd;
	std::mt19937 mt(rnd());

	for (int i = 0; i < n; i++) {
		in[i] = ((int64_t)mt()) <<32 + mt();
	}

	//cudaStatus = randomWithCuda(out, in, random, n);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "randomWithCuda failed!");
		return 1;
	}
	int i;
	for (i = 0; i < n; i++) {
		printf("%lld\n", out[i]);
	}
	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}
	return 0;
}
