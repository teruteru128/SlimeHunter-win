
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include "pch.h"
#include "internal_random.h"

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
