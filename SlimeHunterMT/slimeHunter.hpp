#pragma once

#ifndef SLIME_HUNTER_H
#define SLIME_HUNTER_H
#include <stdint.h>
#include <atomic>

class Config {
public:
	Config();
	~Config();
	std::atomic_uint64_t seed;
protected:
private:
};
class Result
{
public:
	Result(uint64_t worldSeed, int x, int z);
	~Result();
	uint64_t getWorldSeed();
private:
	uint64_t worldSeed;
	int x;
	int z;
};

Result* task(Config* config);

#ifdef _cplusplus
extern "C" {
#endif
	typedef struct point_t {
		int64_t x;
		int64_t z;
	} Point;
	typedef struct rectangleArea_t {
		Point northWest;
		Point southEast;
	} RectangleArea;
	typedef struct rectangleSize_t {
		// North-South width
		uint64_t height;
		// East-west width
		uint64_t width;
	} RectangleSize;
	typedef struct searchConfig_t {
		RectangleArea searchRange;
		RectangleSize cursorSize;
		uint64_t reqMinSlimeChunks;
		uint64_t searchSeeds;
		uint64_t currentSection;
		uint64_t sectionNumber;
	} SearchConfig;
	static inline int isSlimeChunk(uint64_t seed, int chunkX, int chunkZ) {
		uint64_t rnd = seed;
		rnd += (int)(chunkX * 0x5ac0db);
		rnd += (int)(chunkX * chunkX * 0x4c1906);
		rnd += (int)(chunkZ * 0x5f24f);
		rnd += (int)(chunkZ * chunkZ) * 0x4307a7ULL;
		rnd ^= 0x3ad8025fULL;
		setSeed(&rnd, rnd);
		return nextInt(&rnd, 10) == 0;
	}
#ifdef _cplusplus
}
#endif
#endif
