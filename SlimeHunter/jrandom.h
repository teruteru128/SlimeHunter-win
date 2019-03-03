#pragma once

#ifndef JRANDOM_H
#define JRANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common_jrandom.h"

	Random *setSeed(Random*, int64_t);
	int32_t next(Random*, int32_t);
	int64_t nextLong(Random*);
	int32_t nextInt(Random*);
	int32_t nextIntWithRange(Random*, int32_t);

#ifdef __cplusplus
}
#endif
#endif
