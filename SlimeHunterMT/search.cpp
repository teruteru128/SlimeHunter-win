
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "rnd.h"
#include "mcSlimeChunkOracle.h"
#include "search.hpp"
#include <bitset>
#include <atomic>
#include <iostream>
#include <string>
#include <omp.h>
using std::bitset;

bool extracted(std::bitset<625 * 625>* set, int x, int z);

volatile std::atomic_int cont = 1;
std::atomic_uint64_t seed;

Result* task(Config* config) {
	bitset<625 * 625>* set = new bitset<625 * 625>();
	int x = 0;
	int z = 0;
	Result* result = NULL;
	uint64_t worldSeed = 0;
	bool found = false;
	while (cont) {
		worldSeed = seed++;
		for (z = 0; z < 625; z++) {
			for (x = 0; x < 625; x++) {
				set->set(z * 625 + x, isSlimeChunk(worldSeed, x - 312, z - 312));
			}
		}
		found = false;
		for (z = 621; z >= 0; z--) {
			for (x = 621; x >= 0; x--)
			{
				found |= extracted(set, x, z);
			}
		}
		if (found) {
			std::cout << "見つけたー！[" << worldSeed << "]" << std::endl;
			result = new Result(worldSeed, x - 312, z - 312);
			cont = 0;
		}
		if ((worldSeed & 0xfffL) == 0xfffL) {
			std::cout << "done: " << worldSeed << std::endl;
		}
	}

	return result;
}

// TODO &&で繋いだ短絡評価と&で繋いだ投機的実行でどっちのほうが早くなるんだろうか->ベンチマーク
// TODO ベンチマーク->特定のシードではなくランダムなシードを使って計測されなければならない
// TODO 全スレッドで1シードを検査すべきなのか、各スレッドでそれぞれ別のシードを探索すべきなのか(計算粒度の問題)
inline bool extracted(std::bitset<625 * 625>* set, int x, int z) {
	return set->test((z + 3) * 625 + x + 0) && set->test((z + 3) * 625 + x + 1) && set->test((z + 3) * 625 + x + 2) && set->test((z + 3) * 625 + x + 3) &&
		set->test((z + 2) * 625 + x + 0) && set->test((z + 2) * 625 + x + 1) && set->test((z + 2) * 625 + x + 2) && set->test((z + 2) * 625 + x + 3) &&
		set->test((z + 1) * 625 + x + 0) && set->test((z + 1) * 625 + x + 1) && set->test((z + 1) * 625 + x + 2) && set->test((z + 1) * 625 + x + 3) &&
		set->test((z + 0) * 625 + x + 0) && set->test((z + 0) * 625 + x + 1) && set->test((z + 0) * 625 + x + 2) && set->test((z + 0) * 625 + x + 3);
}

int mpsample(void) {
	bitset<625 * 625>* set = new bitset<625 * 625>();
	int pos = 0;
	for (pos = 0; pos < 390625; pos++) {
		set->set(pos, isSlimeChunk(263622805221400ULL, (pos % 625) - 312, (pos / 625) - 312));
	}
	int z = 0;
	int x = 0;
	bool found = false;
#pragma omp parallel for reduction(| : found)
	for (z = 621; z >= 0; z--){
#pragma omp parallel for reduction(| : found)
		for (x = 621; x >= 0; x--) {
			found |= extracted(set, x, z);
		}
	}
	std::cout << found << std::endl;
	return 0;
}

Config::Config() {
	seed = 0ULL;
}
Config::~Config() {}

Result::Result(uint64_t worldSeed, int x, int z)
{
	this->worldSeed = worldSeed;
	this->x = x;
	this->z = z;
}

Result::~Result()
{
}

uint64_t Result::getWorldSeed() {
	return this->worldSeed;
}
