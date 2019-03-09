
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "jrandom.h"
#include "mcSlimeChunkOracle.h"
#include "slimeHunter.h"

int slimeSearch(int64_t initialSeed, const SearchConfig* config) {
	SlimeChunkSeed seed;
	int64_t rndSeed = initialSeed;
#ifdef _DEBUG
	time_t timer;
	struct tm date;
	timer = time(NULL);
	errno_t err = localtime_s(&date, &timer);
	if (err != 0) {
		return EXIT_FAILURE;
	}
	char buf[BUFSIZ];
	size_t len = strftime(buf, BUFSIZ, "%FT%T", &date);
	if (len == 0) {
		return EXIT_FAILURE;
	}

	printf("Initial Seed : %" PRId64" (%s)\n", rndSeed, buf);
	setMCSeed(&seed, -647727624022361904LL);
	{
		int sum = 0;
		for (int z = 50; z < 54; z++) {
			for (int x = 35; x < 39; x++) {
				sum += isSlimeChunkXZ(&seed, x, z);
			}
		}
		printf("%d\n", sum);
	}
#endif

	const int64_t  xMin = config->searchRange.northWest.x, xMax = config->searchRange.southEast.x;
	const int64_t  zMin = config->searchRange.northWest.z, zMax = config->searchRange.southEast.z;
	const int64_t countRangeX = config->searchScope.width;
	const int64_t countRangeZ = config->searchScope.height;
	const int64_t minSlimeChunks = config->reqMinSlimeChunks;
#ifdef _DEBUG
	const uint64_t searchSeeds = config->searchSeeds;
#else
	const uint64_t searchSeeds = config->searchSeeds;
#endif
	int64_t chunkX, x;
	int64_t chunkZ, z;
	uint64_t i = 0;
	int32_t slimeChunkCount = 0;
#ifdef _DEBUG
	int32_t chunkCount = 0;
#else
	int32_t chunkCount = (int32_t)(countRangeX * countRangeZ);
#endif
	int64_t exX = 0;
	int64_t exZ = 0;
	int64_t maxSlimeChunks = 0;
	clock_t start = clock();
#ifdef _DEBUG
	printf("searchSeeds : %" PRIu64"\n", searchSeeds);
#endif
	for (i = 0; i < searchSeeds; i++) {
		setMCSeed(&seed, rndSeed++);

		for (chunkZ = zMin; chunkZ <= zMax; chunkZ += 2) {
			for (chunkX = xMin; chunkX <= xMax; chunkX += 2) {
				if (isSlimeChunkXZ(&seed, chunkX + 2, chunkZ) && isSlimeChunkXZ(&seed, chunkX + 2, chunkZ + 2)) {
					if (isSlimeChunkXZ(&seed, chunkX, chunkZ) && isSlimeChunkXZ(&seed, chunkX, chunkZ + 2)) {
						for (z = -1; z < 1; z++) {
							for (x = -1; x < 1; x++) {
								slimeChunkCount = 4;
#ifdef _DEBUG
								chunkCount = 4;
#endif
								for (exX = 0; exX < countRangeX; exX++) {
									for (exZ = 0; exZ < countRangeZ; exZ++) {
										if ((x + exX) % 2 != 0 || (z + exZ) % 2 != 0) {
											if (isSlimeChunkXZ(&seed, chunkX + exX + x, chunkZ + exZ + z)) {
#ifdef _DEBUG
												chunkCount++;
#endif
												slimeChunkCount++;
											}
										}
									}
								}
								maxSlimeChunks = max(maxSlimeChunks, slimeChunkCount);
								if (slimeChunkCount >= minSlimeChunks) {
									printf("won!,'%"PRId64",%"PRId64",%"PRId64",%"PRId32",%"PRId32"\n", getMCSeed(&seed), (chunkX + x) * 16, (chunkZ + z) * 16, slimeChunkCount, chunkCount);
								}
							}
						}
					}
				}
				else {
					chunkX += 2;
				}
			}
		}
	}
	clock_t finish = clock();
	double seconds = ((double)(finish - start) / CLOCKS_PER_SEC);
	printf("subtotal: This %" PRIu64"/%"PRIu64"sections is %"PRId64"seeds, %.2lfseeds/s, %.2lfs and max is %"PRId64"/%"PRId64".\n", config->currentSection + 1, config->sectionNumber, searchSeeds, searchSeeds / seconds, seconds, maxSlimeChunks, countRangeX * countRangeZ);
	return EXIT_SUCCESS;
}
