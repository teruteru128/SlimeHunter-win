
#include "pch.h"
#define CL_TARGET_OPENCL_VERSION 300
#include "cl.hpp"
#include "search.hpp"
#include <CL/opencl.h>
#include <stdio.h>
int clsample();
static void showPlatformInfo(cl_platform_id platform);
static int showDeviceInfo(cl_device_id device);
static cl_program compileProgram(cl_context gContext, cl_device_id gDevice, const char* fileName);

// bitsetをkernelで扱えるようにunsigned long long* で置き換えーの
// clで集計関数を書きーの
// CPUで集計結果をまとめ～の
int clmain(void) {
	int platformIdx = 0;
	int deviceIdx = 0;
	cl_context gContext = NULL;
	cl_command_queue gCommandQueue = NULL;
	// プラットフォーム取得
	cl_uint platformNumber = 0;
	cl_platform_id platformIds[8];
	checkError("clGetPlatformIDs", clGetPlatformIDs(8, platformIds, &platformNumber));
	cl_platform_id platform = platformIds[platformIdx];

	// デバイス取得
	cl_uint deviceNumber = 0;
	cl_device_id deviceIds[8];
	checkError("clGetDeviceIDs", clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 8, deviceIds, &deviceNumber));
	cl_device_id gDevice = deviceIds[deviceIdx];

	// コンテキスト（メモリ確保などに使用）とコマンドキュー（カーネルの実行などに使用）を作成
	gContext = clCreateContext(NULL, 1, &gDevice, NULL, NULL, NULL);
	gCommandQueue = clCreateCommandQueueWithProperties(gContext, gDevice, 0, NULL);

	// デバイス用プログラムを作成
	cl_int ret = 0;
	cl_program program = compileProgram(gContext, gDevice, "kernel.cl");
	if (program == NULL) {
		std::cerr << "compileProgram error" << std::endl;
	}
	// プログラムをカーネルに変換
	cl_kernel kernel = clCreateKernel(program, "tallySlimeCunks", &ret);
	if (ret != CL_SUCCESS) {
		std::cerr << "error : " << ret << std::endl;
		return 1;
	}

	// デバイス用メモリを確保
	cl_mem gResult = clCreateBuffer(gContext, CL_MEM_READ_WRITE, sizeof(int) * 622 * 622, NULL, &ret);
	if (ret != 0) {
		std::cerr << "clCreateBuffer 1" << ret << std::endl;
		return 1;
	}
	cl_mem gOrigin = clCreateBuffer(gContext, CL_MEM_READ_WRITE, sizeof(uint64_t) * 6104, NULL, &ret);
	if (ret != 0) {
		std::cerr << "clCreateBuffer 2" << ret << std::endl;
		return 1;
	}
	uint64_t* set = (uint64_t*)malloc(sizeof(uint64_t) * 6104);
	if (set == NULL) {
		return 1;
	}
	char* result = (char*)malloc(sizeof(char) * 622 * 622);
	if (result == NULL) {
		return 1;
	}
	int x = 0;
	int z = 0;
	size_t tempZ0 = 0;
	size_t pos = 0;
	size_t wordIndex = 0;
	size_t shift = 0;
	// カーネルの並列実行数を設定
	size_t globalWorkSize[2] = { 622, 622 };
	uint64_t worldSeed = 0;
	while (cont) {
		worldSeed = seed++;
		for (z = 0, tempZ0 = 0; z < 625; z++, tempZ0 += 625) {
			for (x = 0; x < 625; x++)
			{
				pos = tempZ0 + x;
				wordIndex = pos >> 6;
				shift = pos & 0x3f;
				if (isSlimeChunk(worldSeed, x - 312, z - 312)) {
					set[wordIndex] |= (1ULL << shift);
				}
				else {
					set[wordIndex] &= ~(1ULL << shift);
				}
			}
		}

		// host to dev メモリ転送
		checkError("clEnqueueWriteBuffer", clEnqueueWriteBuffer(gCommandQueue, gOrigin, CL_TRUE, 0, sizeof(uint64_t) * 6104, set, 0, NULL, NULL));

		// メモリオブジェクトをカーネル関数の引数にセット
		checkError("clSetKernelArg 1", clSetKernelArg(kernel, 0, sizeof(cl_mem), &gResult));
		checkError("clSetKernelArg 2", clSetKernelArg(kernel, 1, sizeof(cl_mem), &gOrigin));

		// カーネルの呼び出し
		checkError("clEnqueueNDRangeKernel", clEnqueueNDRangeKernel(gCommandQueue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL));

		// dev to host メモリ転送
		checkError("clEnqueueReadBuffer", clEnqueueReadBuffer(gCommandQueue, gResult, CL_TRUE, 0, sizeof(char) * 622 * 622, result, 0, NULL, NULL));

		for (z = 0; z < 622; z++) {
			for (x = 0; x < 622; x++) {
				if (result[z * 622 + x] >= 16) {
					cont = 0;
					std::cout << "found!: " << worldSeed << ", " << x - 312 << ", " << z - 312 << std::endl;
				}
			}
		}
		if ((worldSeed & 0x3fffL) == 0x3fffL) {
			std::cout << "done: " << worldSeed << std::endl;
		}
	}

	clFinish(gCommandQueue);
	// メモリオブジェクト開放
	clReleaseMemObject(gResult);
	clReleaseMemObject(gOrigin);
	clReleaseCommandQueue(gCommandQueue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseContext(gContext);

	return 0;
}

static cl_program compileProgram(cl_context gContext, cl_device_id gDevice, const char* fileName) {
	// プログラムの読み込み
	FILE* fp = NULL;
	errno_t ret = fopen_s(&fp, fileName, "r");
	if (ret != 0)
	{
		perror("fopen_s");
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	const int filesize = ftell(fp);

	fseek(fp, 0, 0);
	char* sourceString = (char*)malloc(filesize);
	if (sourceString == NULL) {
		perror("malloc");
		return NULL;
	}
	size_t sourceSize = fread(sourceString, sizeof(char), filesize, fp);
	fclose(fp);

	// プログラムのコンパイル
	cl_program program = clCreateProgramWithSource(gContext, 1, (const char**)&sourceString, (const size_t*)&sourceSize, NULL);
	cl_int err = clBuildProgram(program, 1, &gDevice, NULL, NULL, NULL);
	// コンパイルに失敗した場合はエラー内容を表示
	if (err != CL_SUCCESS)
	{
		size_t logSize;
		clGetProgramBuildInfo(program, gDevice, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char* buildLog = (char*)malloc((logSize + 1));
		clGetProgramBuildInfo(program, gDevice, CL_PROGRAM_BUILD_LOG, logSize, buildLog, NULL);
		printf("%s", buildLog);
		free(buildLog);
	}
	free(sourceString);
	return program;
}

int clsample()
{
	int platformIdx = 0;
	int deviceIdx = 0;
	// プラットフォーム取得
	cl_uint platformNumber = 0;
	cl_platform_id platformIds[8];
	if (cl_int err = (clGetPlatformIDs(8, platformIds, &platformNumber))) {
		std::cerr << "error [" << "clGetPlatformIDs" << "]: " << err << std::endl;
		return 1;
	};
	std::cout << "platform number" << platformNumber << std::endl;
	cl_platform_id platform = platformIds[platformIdx];
	showPlatformInfo(platform);

	// デバイス取得
	cl_uint deviceNumber = 0;
	cl_device_id deviceIds[8];
	if (cl_int err = (clGetDeviceIDs(platform, 0xFFFFFFFF, 8, deviceIds, &deviceNumber))) {
		std::cerr << "error [" << "clGetDeviceIDs" << "]: " << err << std::endl;
		return 1;
	};
	std::cout << "device numbner:" << deviceNumber << std::endl;
	cl_device_id gDevice = deviceIds[deviceIdx];
	showDeviceInfo(gDevice);
	return 0;
}

static int showPI(cl_platform_id platform, const char* label, cl_platform_info name) {
	size_t val;
	checkError("clGetPlatformInfo", clGetPlatformInfo(platform, name, 0, NULL, &val));
	char* ptr = (char*)malloc(val);
	checkError("clGetPlatformInfo", clGetPlatformInfo(platform, name, val, ptr, &val));
	std::cout << label << ptr << std::endl;
	free(ptr);
	ptr = NULL;
	return 0;
}

static void showPlatformInfo(cl_platform_id platform) {
	showPI(platform, "platform profile: ", CL_PLATFORM_PROFILE);
	showPI(platform, "platform version: ", CL_PLATFORM_VERSION);
	showPI(platform, "platform name: ", CL_PLATFORM_NAME);
	showPI(platform, "platform vendor: ", CL_PLATFORM_VENDOR);
	showPI(platform, "platform extensions: ", CL_PLATFORM_EXTENSIONS);
	showPI(platform, "platform host timer resolution: ", CL_PLATFORM_HOST_TIMER_RESOLUTION);
}

static int showDI(cl_device_id device, const char* label, cl_device_info name) {
	size_t val;
	checkError("clGetDeviceInfo", clGetDeviceInfo(device, name, 0, NULL, &val));
	char* ptr = (char*)malloc(val);
	checkError("clGetDeviceInfo", clGetDeviceInfo(device, name, val, ptr, &val));
	std::cout << label << ptr << std::endl;
	free(ptr);
	return 0;
}

static int showDeviceInfo(cl_device_id device) {
	size_t val;
	checkError("clGetDeviceInfo", clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, NULL, &val));
	cl_device_type* ptr = (cl_device_type*)malloc(sizeof(cl_device_type));
	if (ptr == NULL) {
		// FIXME deadcode
		return 1;
	}
	checkError("clGetDeviceInfo", clGetDeviceInfo(device, CL_DEVICE_TYPE, val, ptr, &val));
	free(ptr);
	return 0;
}
