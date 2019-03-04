
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "random.h"
#include "mcSlimeChunkOracle.h"

int slimeSearch(int64_t initialSeed) {
	SlimeChunkSeed seed;
	int64_t rndSeed = initialSeed;
	int64_t currentSeed = 0;
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
	printf("Initial Seed : %"PRId64" (%s)\n", rndSeed, buf);

	int64_t chunkX, xMin = -313, xMax = 312, x;
	int64_t chunkZ, zMin = -313, zMax = 312, z;
	int64_t countRangeX = 4;
	int64_t countRangeZ = 4;
	int64_t minSlimeChunks = 15;
	uint64_t i = 0;
	int32_t slimeChunkCount = 0;
	int32_t chunkCount = 0;
	long exX = 0;
	long exZ = 0;
	clock_t start = clock();
	uint64_t searchSeeds = 10000000ULL;
	int64_t max = 0;
	for (i = 0; i < searchSeeds; i++) {
		currentSeed = rndSeed++;
		setMCSeed(&seed, currentSeed);

		for (chunkZ = zMin; chunkZ < zMax; chunkZ++) {
			for (chunkX = xMin; chunkX < xMax; chunkX++) {
				if (isSlimeChunkXZ(&seed, chunkX + 2, chunkZ) && isSlimeChunkXZ(&seed, chunkX + 2, chunkZ + 2)) {
					if (isSlimeChunkXZ(&seed, chunkX, chunkZ) && isSlimeChunkXZ(&seed, chunkX, chunkZ + 2)) {
						for (z = -1; z < 1; z++) {
							for (x = -1; x < 1; x++) {
								slimeChunkCount = 4;
								chunkCount = 4;
								for (exX = 0; exX < countRangeX; exX++) {
									for (exZ = 0; exZ < countRangeZ; exZ++) {
										if ((x + exX) % 2 != 0 || (z + exZ) % 2 != 0) {
											chunkCount++;
											if (isSlimeChunkXZ(&seed, chunkX + exX + x, chunkZ + exZ + z)) {
												slimeChunkCount++;
											}
										}
									}
								}
								max = slimeChunkCount > max ? slimeChunkCount : max;
								if (slimeChunkCount >= minSlimeChunks) {
									printf("'%"PRId64",%"PRId64",%"PRId64",%"PRId32",%"PRId32"\n", currentSeed, (chunkX + x) * 16, (chunkZ + z) * 16, slimeChunkCount, chunkCount);
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
	printf("%"PRId64"seeds, %.2lfseeds/s, %.2lfs and max is %"PRId64"/%"PRId64"\n", searchSeeds, searchSeeds / seconds, seconds, max, countRangeX * countRangeZ);
	return EXIT_SUCCESS;
}

