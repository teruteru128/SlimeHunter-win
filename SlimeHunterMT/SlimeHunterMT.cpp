// SlimeHunterMT.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

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
#include "slimeHunter.hpp"
#include <crtdbg.h>
#include <ctime>
#include <stdlib.h>
#include <future>

// C++標準の疑似乱数生成器とbitsetとatomicで作りたい
//
#define FILENAMELEN 32
int main(int argc, char* argv[], char* env[])
{
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
	std::cout << "starting seed: " + std::to_string(start) << std::endl;
	std::cout << "starting thread: " + std::to_string(threadNum) << std::endl;

	Config config;
	config.seed = start;

	std::vector<std::future<Result*>> futures(threadNum);
	for (int i = 0; i < threadNum; i++) {
		futures[i] = std::async(task, &config);
	}
	Result* r = NULL;
	for (int i = 0; i < threadNum; i++) {
		r = futures[i].get();
		if (r != NULL) {
			std::cout << r->getWorldSeed() << std::endl;
		}
	}
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
