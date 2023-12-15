// SlimeHunterMT.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#define CL_TARGET_OPENCL_VERSION 300
#include "cl.hpp"
#include "main.hpp"
#include "rnd.h"
#include "search.hpp"
#include "omp.hpp"
#include <chrono>
#include <crtdbg.h>
#include <ctime>
#include <CL/opencl.h>
#include <future>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#ifdef __linux
#include <unistd.h>
#elif __MINGW32__
#include <windows.h>
#include <winbase.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

static void handler(int signum) {
	cont = 0;
}

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
	int threadNum = 1;
	int clmainflag = 0;
	uint64_t start = 0;
	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--thread") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc) {
			threadNum = (int)strtoull(argv[i + 1], NULL, 10);
		}
		else if ((strcmp(argv[i], "--start") || strcmp(argv[i], "-s") == 0) && i + 1 < argc) {
			start = strtoull(argv[i + 1], NULL, 10);
		}
		else if (strcmp(argv[1], "--clmain") == 0) {
			clmainflag = 1;
		}
	}
	// ctrl-cで中断するためのハンドラーを設定(フラグを降ろす用)
	if (signal(SIGINT, handler) == SIG_ERR) {
		return 1;
	}
	std::atomic_uint64_t* seedptr = new std::atomic_uint64_t(start);
	if (seedptr == NULL) {
		perror("seedptr is null");
		return 1;
	}
	if (clmainflag) {
		return clmain(seedptr);
	}
	std::cout << "starting seed: " << start << std::endl;
	std::cout << "starting thread: " << threadNum << std::endl;

	Config config(seedptr);

	std::vector<std::future<Result*>> futures(threadNum);
	auto startTime = std::chrono::steady_clock::now();
	for (int i = 0; i < threadNum; i++) {
		futures[i] = std::async(task, &config);
	}
	Result* r = NULL;
	bool isAllNull = 1;
	for (int i = 0; i < threadNum; i++) {
		r = futures[i].get();
		if (r != NULL) {
			std::cout << "見つかりました" << r->getWorldSeed() << ", " << r->getX() << ", " << r->getZ() << std::endl;
			delete r;
			isAllNull = 0;
		}
	}
	auto finishTime = std::chrono::steady_clock::now();
	if (isAllNull) {
		std::cout << "見つかりませんでした" << std::endl;
	}
	auto seeddiff = *seedptr - start;
	auto timediff = std::chrono::duration_cast<std::chrono::nanoseconds>(finishTime - startTime).count() / 1e9;
	std::cout << "計算時間: " << timediff << " 秒" << std::endl;
	std::cout << "進捗: " << seeddiff << " seeds" << std::endl;
	std::cout << "計算効率: " << seeddiff / timediff << " seeds/seconds" << std::endl;
	std::cout << "次回開始シード: " << *seedptr << std::endl;
	system("PAUSE");
	return EXIT_SUCCESS;
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
