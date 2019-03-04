#pragma once

#ifndef MC_SLIME_CHUNK_ORACLE_H
#define MC_SLIME_CHUNK_ORACLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jrandom.h"
#include "commonMcSlimeChunkOracle.h"
#include <stdbool.h>

	int64_t getMCSeed(SlimeChunkSeed*, int32_t, int32_t);
	void setMCSeed(SlimeChunkSeed*, int64_t);
	bool isSlimeChunkXZ(SlimeChunkSeed *, int64_t, int64_t);
	bool isSlimeChunk(Random *);


#ifdef __cplusplus
}
#endif
#endif
