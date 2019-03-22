// SlimeHunterMT.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <cstdint>
#include <cinttypes>
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
#include "jrandom.h"
#include "slimeHunter.h"
#include <crtdbg.h>

extern "C" {
	int slimeSearch(int64_t, const SearchConfig*);
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	printf("debug mode\n");
#endif
	std::random_device rnd;
	int64_t initialSeed;
	// 4300000ULL
	SearchConfig config = { {{-313,-313},{312,312}},{5,5}, 21, 0x400000ULL , 0, 0x100ULL};
	time_t timer;
	struct tm date;
	timer = time(NULL);
	errno_t err = localtime_s(&date, &timer);
	if (err != 0) {
		return EXIT_FAILURE;
	}
	char buf[BUFSIZ];
	size_t len = strftime(buf, BUFSIZ, "%FT%T", &date);
	if (len == 0) {
		return EXIT_FAILURE;
	}
	printf("start : %s\n", buf);
	uint64_t sectionNumber = config.sectionNumber;
#ifdef _DEBUG
	sectionNumber = config.sectionNumber = 4;
	config.searchSeeds /= 100;
#endif
	for (uint64_t i = 0; i < sectionNumber; i++) {
		config.currentSection = i;
		initialSeed = ((int64_t)rnd() << 32) + rnd();
		slimeSearch(initialSeed, &config);
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
