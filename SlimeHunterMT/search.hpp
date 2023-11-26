
#pragma once
#ifndef SLIME_HUNTER_H
#define SLIME_HUNTER_H
#include <stdint.h>
#include "rnd.h"
#include <atomic>

class Config {
public:
	Config();
	~Config();
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
extern volatile std::atomic_int cont;
extern volatile std::atomic_uint64_t seed;

int mpsample(void);
#ifdef _cplusplus
extern "C" {
#endif
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
