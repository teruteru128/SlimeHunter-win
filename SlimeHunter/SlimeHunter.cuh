#pragma once
#ifndef SLIME_HUNTER_H
#define SLIME_HUNTER_H 1

#include <stdint.h>
void searchSeed(int64_t);
typedef struct SearchResult_t {
	int64_t seed = 0;
	int32_t used = 0;
	int16_t chunkX = 0;
	int16_t chunkZ = 0;
} SearchResult;
__global__ void searchSlimeKernel(int64_t);
// いあ！いあ！
#endif
