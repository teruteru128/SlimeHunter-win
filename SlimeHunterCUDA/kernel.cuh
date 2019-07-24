#pragma once

#ifndef INTERNAL_JRANDOM_H
#define INTERNAL_JRANDOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MULTIPLIER 0x5DEECE66DULL
#define ADDEND 0xBULL
#define MASK 0xFFFFFFFFFFFFULL

	typedef struct random_t {
		int64_t seed;
	} Random;


#ifdef __cplusplus
}
#endif
#endif
