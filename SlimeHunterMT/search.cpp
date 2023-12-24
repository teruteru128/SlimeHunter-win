
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
constexpr int BLOCK = 25;

Result* task(Config* config) {
	int skip = 1;
	int x = 0;
	// int xx = 0;
	// int minXX = 0;
	const int minX = config->getMinX();
	const int maxX = config->getMaxX();
	int z = 0;
	const int minZ = config->getMinZ();
	const int maxZ = config->getMaxZ();
	uint64_t worldSeed = 0;
	const int limit = maxX - 4;
	std::atomic_uint64_t* ptr = config->getSeed();
	while (cont) {
		worldSeed = std::atomic_fetch_add(ptr, 1);
		z = maxZ;
		while (z >= minZ) {
			skip = 1;
			x = maxX;
			while (x >= minX) {
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
				return new Result(worldSeed, x << 4, z << 4);
			}
			// 4個未満ならスキップ
			z -= ((skip) ? 5 : 1);
		}
		// スキップのヒット率を上げるためにZ方向だけブロッキングする
		/*
		x = maxX;
		while (x >= minX) {
			z = maxZ;
			while (z >= minZ) {
				xx = std::min(x, limit);
				minXX = xx - BLOCK;
				skip = 1;
				while (xx >= minXX) {
					if (!isSlimeChunk(worldSeed, xx, z)) {
						xx -= 5;
						continue;
					}
					if (!isSlimeChunk(worldSeed, xx + 1, z)) {
						xx -= 4;
						continue;
					}
					if (!isSlimeChunk(worldSeed, xx + 2, z)) {
						xx -= 3;
						continue;
					}
					if (!isSlimeChunk(worldSeed, xx + 3, z)) {
						xx -= 2;
						continue;
					}
					if (!isSlimeChunk(worldSeed, xx + 4, z)) {
						xx -= 1;
						continue;
					}
					skip = 0;
					if (!(isSlimeChunk(worldSeed, xx, z + 1) && isSlimeChunk(worldSeed, xx, z + 2) && isSlimeChunk(worldSeed, xx, z + 3) && isSlimeChunk(worldSeed, xx, z + 4))) {
						xx -= 5;
						continue;
					}
					if (!(isSlimeChunk(worldSeed, xx + 1, z + 1) && isSlimeChunk(worldSeed, xx + 1, z + 2) && isSlimeChunk(worldSeed, xx + 1, z + 3) && isSlimeChunk(worldSeed, xx + 1, z + 4))) {
						xx -= 4;
						continue;
					}
					if (!(isSlimeChunk(worldSeed, xx + 2, z + 1) && isSlimeChunk(worldSeed, xx + 2, z + 2) && isSlimeChunk(worldSeed, xx + 2, z + 3) && isSlimeChunk(worldSeed, xx + 2, z + 4))) {
						xx -= 3;
						continue;
					}
					if (!(isSlimeChunk(worldSeed, xx + 3, z + 1) && isSlimeChunk(worldSeed, xx + 3, z + 2) && isSlimeChunk(worldSeed, xx + 3, z + 3) && isSlimeChunk(worldSeed, xx + 3, z + 4))) {
						xx -= 2;
						continue;
					}
					if (!(isSlimeChunk(worldSeed, xx + 4, z + 1) && isSlimeChunk(worldSeed, xx + 4, z + 2) && isSlimeChunk(worldSeed, xx + 4, z + 3) && isSlimeChunk(worldSeed, xx + 4, z + 4))) {
						xx -= 1;
						std::cout << "惜しい!" << worldSeed << ", " << (xx << 4) << ", " << (z << 4) << std::endl;
						continue;
					}
					cont = 0;
					std::cout << "見つけたー！[" << worldSeed << ", " << (xx << 4) << ", " << (z << 4) << "]" << std::endl;
					return new Result(worldSeed, xx << 4, z << 4);
				}
				// 4個未満ならスキップ
				z -= ((skip) ? 5 : 1);
			}
			x -= BLOCK;
		}
		*/
	}

	return NULL;
}

Config::Config(std::atomic_uint64_t* seed) : Config(seed, -312, 308, -312, 308) {
}

Config::Config(std::atomic_uint64_t* seed, int minX, int maxX, int minZ, int maxZ) :
	seed(seed), minX(minX), maxX(maxX), minZ(minZ), maxZ(maxZ) {
}

Config::~Config() {}

std::atomic_uint64_t* Config::getSeed() {
	return this->seed;
}

int Config::getMinX() {
	return this->minX;
}

int Config::getMaxX() {
	return this->maxX;
}

int Config::getMinZ() {
	return this->minZ;
}

int Config::getMaxZ() {
	return this->maxZ;
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
