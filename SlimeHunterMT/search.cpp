
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

Result* task(Config* config) {
	int skip = 1;
	int x = 0;
	int z = 0;
	uint64_t worldSeed = 0;
	std::atomic_uint64_t* ptr = config->getSeed();
	while (cont) {
		worldSeed = std::atomic_fetch_add(ptr, 1);
		// TODO z = 60000000; z >= 0; z--
		z = 308;
		while (z >= -312) {
			skip = 1;
			// TODO x = 60000000; x >= 0; x--
			x = 308;
			while (x >= -312) {
				// TODO x - 30000000, z - 30000000
				if (!isSlimeChunk(worldSeed, x, z)) {
					x -= 5;
					continue;
				}
				if (!isSlimeChunk(worldSeed, x + 1, z)) {
					x -= 4;
					continue;
				}
				if (!isSlimeChunk(worldSeed, x + 2, z)) {
					x -= 3;
					continue;
				}
				if (!isSlimeChunk(worldSeed, x + 3, z)) {
					x -= 2;
					continue;
				}
				if (!isSlimeChunk(worldSeed, x + 4, z)) {
					x -= 1;
					continue;
				}
				skip = 0;
				if (!(isSlimeChunk(worldSeed, x, z + 1) && isSlimeChunk(worldSeed, x, z + 2) && isSlimeChunk(worldSeed, x, z + 3) && isSlimeChunk(worldSeed, x, z + 4))) {
					x -= 5;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x + 1, z + 1) && isSlimeChunk(worldSeed, x + 1, z + 2) && isSlimeChunk(worldSeed, x + 1, z + 3) && isSlimeChunk(worldSeed, x + 1, z + 4))) {
					x -= 4;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x + 2, z + 1) && isSlimeChunk(worldSeed, x + 2, z + 2) && isSlimeChunk(worldSeed, x + 2, z + 3) && isSlimeChunk(worldSeed, x + 2, z + 4))) {
					x -= 3;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x + 3, z + 1) && isSlimeChunk(worldSeed, x + 3, z + 2) && isSlimeChunk(worldSeed, x + 3, z + 3) && isSlimeChunk(worldSeed, x + 3, z + 4))) {
					x -= 2;
					continue;
				}
				if (!(isSlimeChunk(worldSeed, x + 4, z + 1) && isSlimeChunk(worldSeed, x + 4, z + 2) && isSlimeChunk(worldSeed, x + 4, z + 3) && isSlimeChunk(worldSeed, x + 4, z + 4))) {
					x -= 1;
					std::cout << "惜しい!" << worldSeed << ", " << (x << 4) << ", " << (z << 4) << std::endl;
					continue;
				}
				cont = 0;
				std::cout << "見つけたー！[" << worldSeed << ", " << (x << 4) << ", " << (z << 4) << "]" << std::endl;
				// delete set;
				return new Result(worldSeed, x << 4, z << 4);
			}
			// 4個未満ならスキップ
			if (skip)
			{
				z -= 5;
				continue;
			}
			z -= 1;
		}
	}

	return NULL;
}

Config::Config(std::atomic_uint64_t* seed) {
	this->seed = seed;
}
Config::~Config() {}
std::atomic_uint64_t* Config::getSeed() {
	return this->seed;
}
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
