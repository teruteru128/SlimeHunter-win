#pragma once

#include "pch.h"
#include "rnd.h"
#include <stdint.h>

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
