
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

#include "jrandom.cuh"

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
