// SlimeHunterMT.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#define CL_TARGET_OPENCL_VERSION 300

#include "pch.h"
#include <iostream>
#include <random>
#include <thread>
#ifdef __linux
#include <unistd.h>
#elif __MINGW32__
#include <windows.h>
#include <winbase.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include "rnd.h"
#include "search.hpp"
#include <crtdbg.h>
#include <ctime>
#include <stdlib.h>
#include <signal.h>
#include <future>
#include <CL/cl.h>
#include "main.h"
#define checkError(openclFunction)          \
    if (cl_int err = (openclFunction))      \
    {                                       \
        printf("error : %d\n", err);        \
    }
static void handler(int signum) {
	cont = 0;
}
int clsample();
static void showPlatformInfo(cl_platform_id platform);
static void showDeviceInfo(cl_device_id device);

//
#define FILENAMELEN 32
int main(int argc, char* argv[], char* env[])
{
	// サンプルへリンク
	if (argc > 1 && strcmp(argv[1], "--mp") == 0) {
		return mpsample();
	}
	if (argc > 1 && strcmp(argv[1], "--cl") == 0) {
		return clsample();
	}

	// スレッド数と初期シードを設定
	unsigned long long start = 0;
	int threadNum = 1;
	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--thread") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc) {
			threadNum = (int)strtoull(argv[i + 1], NULL, 10);
		}
		else if ((strcmp(argv[i], "--start") || strcmp(argv[i], "-s") == 0) && i + 1 < argc) {
			start = strtoull(argv[i + 1], NULL, 10);
		}
	}
	// ctrl-cで中断するためのハンドラーを設定(フラグを降ろす用)
	if (signal(SIGINT, handler) == SIG_ERR) {
		return 1;
	}
	std::cout << "starting seed: " << start << std::endl;
	std::cout << "starting thread: " << threadNum << std::endl;

	Config config;
	seed = start;

	std::vector<std::future<Result*>> futures(threadNum);
	for (int i = 0; i < threadNum; i++) {
		futures[i] = std::async(task, &config);
	}
	Result* r = NULL;
	bool isAllNull = 1;
	for (int i = 0; i < threadNum; i++) {
		r = futures[i].get();
		if (r != NULL) {
			std::cout << "見つかりました" << r->getWorldSeed() << std::endl;
			delete r;
			isAllNull = 0;
		}
	}
	if (isAllNull) {
		std::cout << "見つかりませんでした" << std::endl;
	}
	std::cout << "次回開始シード: " << seed << std::endl;
	system("PAUSE");
	return EXIT_SUCCESS;
}

static int clsample()
{
	int platformIdx = 0;
	int deviceIdx = 0;
	// プラットフォーム取得
	cl_uint platformNumber = 0;
	cl_platform_id platformIds[8];
	checkError(clGetPlatformIDs(8, platformIds, &platformNumber));
	std::cout << "platform number" << platformNumber << std::endl;
	cl_platform_id platform = platformIds[platformIdx];
	showPlatformInfo(platform);

	// デバイス取得
	cl_uint deviceNumber = 0;
	cl_device_id deviceIds[8];
	checkError(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 8, deviceIds, &deviceNumber));
	std::cout << "device numbner:" << deviceNumber << std::endl;
	cl_device_id gDevice = deviceIds[deviceIdx];
	showDeviceInfo(gDevice);
	return 0;
}

static void showPI(cl_platform_id platform, const char* label, cl_platform_info name) {
	size_t val;
	checkError(clGetPlatformInfo(platform, name, 0, NULL, &val));
	char* ptr = (char*)malloc(val);
	checkError(clGetPlatformInfo(platform, name, val, ptr, &val));
	std::cout << label << ptr << std::endl;
	free(ptr);
	ptr = NULL;
}

static void showPlatformInfo(cl_platform_id platform) {
	showPI(platform, "platform profile: ", CL_PLATFORM_PROFILE);
	showPI(platform, "platform version: ", CL_PLATFORM_VERSION);
	showPI(platform, "platform name: ", CL_PLATFORM_NAME);
	showPI(platform, "platform vendor: ", CL_PLATFORM_VENDOR);
	showPI(platform, "platform extensions: ", CL_PLATFORM_EXTENSIONS);
	showPI(platform, "platform host timer resolution: ", CL_PLATFORM_HOST_TIMER_RESOLUTION);
}

static void showDI(cl_device_id device, const char* label, cl_device_info name) {
	size_t val;
	checkError(clGetDeviceInfo(device, name, 0, NULL, &val));
	char* ptr = (char*)malloc(val);
	checkError(clGetDeviceInfo(device, name, val, ptr, &val));
	std::cout << label << ptr << std::endl;
	free(ptr);
}

static void showDeviceInfo(cl_device_id device) {
	size_t val;
	checkError(clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, NULL, &val));
	cl_device_type* ptr = (cl_device_type*)malloc(sizeof(cl_device_type));
	if (ptr == NULL) {
		// FIXME deadcode
		return;
	}
	checkError(clGetDeviceInfo(device, CL_DEVICE_TYPE, val, ptr, &val));
	std::cout << "device type: " << *ptr << std::endl;
	free(ptr);
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
