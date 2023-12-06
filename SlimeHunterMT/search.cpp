
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
	size_t pos = 0;
	uint64_t worldSeed = 0;
	while (cont) {
		worldSeed = seed++;
		for (z = 621, tempZ0 = 388125; z >= 0; z--, tempZ0 -= 625) {
			lineCombo = 0;
			lineComboMax = 1;
			// 時間と空間のトレードオフ
			// 連続でスライムチャンクが並んでる個数を数える
			// ビット演算でスライムチャンクが4つ連続しているかだけ返す
			for (x = 0; x < 625; x++) {
				lineCombo = ((lineCombo << 1) | (int)((*set)[tempZ0 + x] = isSlimeChunk(worldSeed, x - 312, z - 312))) & 15;
				lineComboMax &= lineCombo != 15;
			}
			// 4個未満ならスキップ
			if (lineComboMax)
			{
				z -= 3;
				tempZ0 -= 1875;
				continue;
			}
			for (x = 621; x >= 0; x--)
			{
				pos = tempZ0 + x;
				if (!set->test(pos)) {
					x -= 3;
					continue;
				}
				if (!set->test(pos + 1)) {
					x -= 2;
					continue;
				}
				if (!set->test(pos + 2)) {
					x -= 1;
					continue;
				}
				if (!set->test(pos + 3)) {
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x - 312, z - 312 + 1) && isSlimeChunk(worldSeed, x - 312, z - 312 + 2) && isSlimeChunk(worldSeed, x - 312, z - 312 + 3))) {
					x -= 3;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x - 312 + 1, z - 312 + 1) && isSlimeChunk(worldSeed, x - 312 + 1, z - 312 + 2) && isSlimeChunk(worldSeed, x - 312 + 1, z - 312 + 3))) {
					x -= 2;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x - 312 + 2, z - 312 + 1) && isSlimeChunk(worldSeed, x - 312 + 2, z - 312 + 2) && isSlimeChunk(worldSeed, x - 312 + 2, z - 312 + 3))) {
					x -= 1;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x - 312 + 3, z - 312 + 1) && isSlimeChunk(worldSeed, x - 312 + 3, z - 312 + 2) && isSlimeChunk(worldSeed, x - 312 + 3, z - 312 + 3))) {
					continue;
				}
				cont = 0;
				std::cout << "見つけたー！[" << worldSeed << "]" << std::endl;
				// delete set;
				return new Result(worldSeed, (x - 312) << 4, (z - 312) << 4);
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
