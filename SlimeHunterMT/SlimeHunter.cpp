
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "jrandom.hpp"
#include "mcSlimeChunkOracle.h"
#include "slimeHunter.hpp"
#include <bitset>
#include <atomic>
#include <iostream>
#include <string>
using std::bitset;

bool extracted(bitset<625 * 625>* set, int x, int z);
uint64_t calcSeed(uint64_t worldSeed, int x, int z);

Result* task(Config* config) {
	bitset<625 * 625>* set = new bitset<625 * 625>();
	random::Random* random = new random::Random(0);
	int x = 0;
	int z = 0;
	Result* result = NULL;
	uint64_t worldSeed = 0;
	std::atomic_uint64_t* seed = &config->seed;
	bool found = false;
	while (result == NULL) {
		worldSeed = (*seed)++;
		for (z = 0; z < 625; z++) {
			for (x = 0; x < 625; x++) {
				random->setSeed(calcSeed(worldSeed, x - 312, z - 312));
				set->set(z * 625 + x, random->nextInt(10) == 0);
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
			std::cout << "‚¤‚ñ‚¿[I[" + std::to_string(worldSeed) + "]" << std::endl;
			result = new Result(worldSeed, x, z);
		}
		if ((worldSeed & 0xfffL) == 0xfffL) {
			std::cout << "done: " + std::to_string(worldSeed) << std::endl;
		}
	}

	return result;
}

bool extracted(bitset<625 * 625>* set, int x, int z) {
	return set->test((z + 3) * 625 + x + 3) && set->test((z + 3) * 625 + x + 2) && set->test((z + 3) * 625 + x + 1) && set->test((z + 3) * 625 + x + 0) &&
		set->test((z + 2) * 625 + x + 3) && set->test((z + 2) * 625 + x + 2) && set->test((z + 2) * 625 + x + 1) && set->test((z + 2) * 625 + x + 0) &&
		set->test((z + 1) * 625 + x + 3) && set->test((z + 1) * 625 + x + 2) && set->test((z + 1) * 625 + x + 1) && set->test((z + 1) * 625 + x + 0) &&
		set->test((z + 0) * 625 + x + 3) && set->test((z + 0) * 625 + x + 2) && set->test((z + 0) * 625 + x + 1) && set->test((z + 0) * 625 + x + 0);
}

uint64_t calcSeed(uint64_t worldSeed, int x, int z) {
	return worldSeed + (int)(x * x * 0x4c1906) + x * 0x5ac0db + (int)(z * z) * 0x4307a7L + (int)(z * 0x5f24f)
		^ 0x3ad8025fL;
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
