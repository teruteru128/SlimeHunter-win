
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include "pch.h"
#include "internal_random.h"

namespace random{
	class Random
	{
	public:
		Random();
		Random(long);
		~Random();
		void setSeed(uint64_t seed);
		int nextInt();
		int nextInt(int bound);
	private:
		int64_t seed;
		int next(int bits);
		static uint64_t initialScramble(uint64_t seed);
	};

}

#ifdef __cplusplus
extern "C"{
#endif

typedef struct random_t{
  int64_t seed;
} Random;

Random *setSeed(Random*, int64_t);
int64_t nextLong(Random*);
int32_t nextInt(Random*);
int32_t nextIntWithRange(Random*, int32_t);
#ifdef __cplusplus
}
#endif
#endif
