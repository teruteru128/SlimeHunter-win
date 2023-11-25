
#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include "pch.h"
#include "internal_random.h"

#ifdef __cplusplus
extern "C" {
#endif

	static inline void setSeed(uint64_t* seed, int64_t value) {
		*seed = (value ^ 0x5deece66d) & ((1ULL << 48) - 1);
	}
	static inline int32_t next(uint64_t* seed, const int bits) {
		*seed = (*seed * 0x5deece66d + 0xb) & ((1ULL << 48) - 1);
		return (int)((int64_t)*seed >> (48 - bits));
	}

	static inline int32_t nextInt(uint64_t* seed, const int32_t n) {
		int bits, val;
		const int m = n - 1;

		if ((m & n) == 0) {
			uint64_t x = n * (uint64_t)next(seed, 31);
			return (int)((int64_t)x >> 31);
		}

		do {
			bits = next(seed, 31);
			val = bits % n;
		} while (bits - val + m < 0);
		return val;
	}
#ifdef __cplusplus
}
#endif
#endif
