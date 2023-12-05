
#include "pch.h"
#define CL_TARGET_OPENCL_VERSION 300
#include "mcSlimeChunkOracle.h"
#include "rnd.h"
#include "search.hpp"
#include <algorithm>
#include <atomic>
#include <bitset>
#include <CL/opencl.h>
#include <inttypes.h>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
using std::bitset;

volatile std::atomic_int cont = 1;
volatile std::atomic_uint64_t seed;

Result* task(Config* config) {
	auto set = new bitset<625 * 625>();
	int lineCombo = 0;
	int lineComboMax = 1;
	int x = 0;
	int z = 0;
	size_t tempZ0;
	size_t tempZ0_r;
	size_t pos = 0;
	uint64_t worldSeed = 0;
	bool found = false;
	while (cont) {
		worldSeed = seed++;
		for (z = 0, tempZ0 = 0; z < 625; z++, tempZ0 += 625) {
			for (x = 0; x < 625; x++) {
				set->set(tempZ0 + x, isSlimeChunk(worldSeed, x - 312, z - 312));
			}
		}
		found = false;
		for (tempZ0 = 0; tempZ0 < 388125; tempZ0 += 625) {
			tempZ0_r = 388125 - tempZ0;
			lineCombo = 0;
			lineComboMax = 1;
			// 時間と空間のトレードオフ
			// 連続でスライムチャンクが並んでる個数を数える
			// ビット演算でスライムチャンクが4つ連続しているかだけ返す
			for (x = 0; lineComboMax && x < 625; x++) {
				lineCombo = ((lineCombo << 1) | (int)set->test(tempZ0_r + x)) & 15;
				lineComboMax &= lineCombo != 15;
			}
			// 4個未満ならスキップ
			if (lineComboMax)
			{
				tempZ0 += 1875;
				continue;
			}
			for (x = 621; x >= 0; x--)
			{
				pos = tempZ0_r + x;
				if (!(set->test(pos + 1875) && set->test(pos + 1250) && set->test(pos + 625) && set->test(pos))) {
					x -= 3;
					continue;
				}
				if (!(set->test(pos + 1876) && set->test(pos + 1251) && set->test(pos + 626) && set->test(pos + 1))) {
					x -= 2;
					continue;
				}
				if (!(set->test(pos + 1877) && set->test(pos + 1252) && set->test(pos + 627) && set->test(pos + 2))) {
					x -= 1;
					continue;
				}
				if (!(set->test(pos + 1878) && set->test(pos + 1253) && set->test(pos + 628) && set->test(pos + 3))) {
					continue;
				}
				if (extracted(set, pos)) {
					cont = 0;
					std::cout << "見つけたー！[" << worldSeed << "]" << std::endl;
					// delete set;
					return new Result(worldSeed, x - 312, (int)(tempZ0_r / 625) - 312);
				}
			}
		}
	}

	return NULL;
}

Config::Config() {
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

uint64_t Result::getWorldSeed() const {
	return this->worldSeed;
}

int Result::getX() const {
	return this->x;
}

int Result::getZ() const {
	return this->z;
}
