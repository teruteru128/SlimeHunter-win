#pragma once

#ifndef SLIME_HUNTER_H
#define SLIME_HUNTER_H
#include <stdint.h>
#ifdef _cplusplus
extern "C" {
#endif
	typedef struct point_t {
		int64_t x;
		int64_t z;
	} Point;
	typedef struct rectangle_t {
		Point northWest;
		Point southEast;
	} RectangleArea;
	typedef struct rectangleSize_t {
		// North-South width
		uint64_t height;
		// East-west width
		uint64_t width;
	} RectangleSize;
	typedef struct searchConfig_t {
		RectangleArea searchRange;
		RectangleSize searchScope;
		uint64_t reqMinSlimeChunks;
		uint64_t searchSeeds;
		uint64_t currentSection;
		uint64_t sectionNumber;
	} SearchConfig;
#ifdef _cplusplus
}
#endif
#endif
