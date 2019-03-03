#pragma once

#ifndef COMMON_MC_SLIME_CHUNK_ORACLE_H
#define COMMON_MC_SLIME_CHUNK_ORACLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jrandom.cuh"
	typedef struct SlimeChunkSeed_t {
		int64_t seed;
		Random rnd;
	} SlimeChunkSeed;

#ifdef __cplusplus
}
#endif
#endif
