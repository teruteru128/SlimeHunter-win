
#include "pch.h"
#include "internal_random.h"
#include "jrandom.hpp"
#include <stdexcept>

namespace random {
	Random::Random()
	{
		this->seed = 0ULL;
	}
	Random::Random(long seed)
	{
		this->seed = Random::initialScramble(seed);
	}

	Random::~Random()
	{
	}
	void Random::setSeed(uint64_t seed) {
		seed = initialScramble(seed);
	}
	int Random::nextInt() {
		return next(32);
	}
	int Random::nextInt(int bound) {
		if (bound <= 0) {
			throw std::invalid_argument("bad bound");
		}
		int r = this->next(31);
		int m = bound - 1;
		if ((bound & m) == 0) {
			r = (int)((bound * (long)r) >> 31);
		}
		else {
			for (int u = r; u - (r = u % bound) + m < 0; u = next(31));
		}
		return r;
	}
	int Random::next(int bits) {
		uint64_t oldseed, nextseed;
		uint64_t seed = this->seed;
		oldseed = seed;
		nextseed = (oldseed * MULTIPLIER + ADDEND) & MASK;
		this->seed = nextseed;
		return (int)(nextseed >> (48 - bits));
	}
	uint64_t Random::initialScramble(uint64_t seed) {
		return (seed ^ MULTIPLIER) & MASK;
	}
}


extern "C" {
	static int64_t initialScramble(uint64_t seed) {
		return (seed ^ MULTIPLIER) & MASK;
	}

	Random* setSeed(Random* rnd, int64_t seed) {
		rnd->seed = initialScramble(seed);
		return rnd;
	}

	static int32_t next(Random* rnd, int32_t bits) {
		uint64_t oldseed, nextseed;
		uint64_t seed = rnd->seed;
		oldseed = seed;
		nextseed = (oldseed * MULTIPLIER + ADDEND) & MASK;
		rnd->seed = nextseed;
		return (uint32_t)(nextseed >> (48 - bits));
	}

	int64_t nextLong(Random* rnd) {
		return ((int64_t)next(rnd, 32) << 32) + next(rnd, 32);
	}

	int32_t nextInt(Random* rnd) {
		return next(rnd, 32);
	}

	int32_t nextIntWithRange(Random* rnd, int32_t bound) {
		if (bound <= 0) {
			// err!
			return 0;
		}
		int32_t r = next(rnd, 31);
		int32_t m = bound - 1;
		if ((bound & m) == 0) {
			r = (int32_t)((bound * (int64_t)r) >> 31);
		}
		else {
			int32_t u;
			for (u = r; u - (r = u % bound) + m < 0; u = next(rnd, 31));
		}
		return r;
	}

}
