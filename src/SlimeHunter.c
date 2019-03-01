
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "random.h"
#include "mcSlimeChunkOracle.h"
#include "kernel.cuh"

void searchSeed(int64_t);

int slimeHunter(int argc, char* argv[]) {
	Random random;
	setSeed(&random, 0);
	printf("%u\n", next(&random, 32));
	printf("%u\n", nextInt(&random));
	printf("%u\n", nextIntWithRange(&random, 10));
	printf("%"PRId64"\n", nextLong(&random));

	FILE* fp;
	char* rpath = "/dev/urandom";
	fopen_s(&fp, rpath, "rb");
	if (fp == NULL) {
		return EXIT_FAILURE;
	}
	int64_t rndSeed = 0;
	int64_t currentSeed = 0;
	(void)fread(&rndSeed, sizeof(rndSeed), 1, fp);

	printf("Initial Seed : %"PRId64"\n", rndSeed);
	searchSeed(rndSeed);
	return EXIT_SUCCESS;
}

void searchSeed(int64_t rndSeed) {
	SlimeChunkSeed seed;
	int64_t currentSeed = 1;
	int32_t chunkX, xMin = -313, xMax = 312;
	int32_t chunkZ, zMin = -313, zMax = 312;
	int32_t countRangeX = 4;
	int32_t countRangeZ = 4;
	int32_t minSlimeChunks = 14;
	uint64_t i = 0;
	int32_t slimeChunkCount = 0;
	int32_t chunkCount = 0;
	int32_t exX = 0;
	int32_t exZ = 0;
	int32_t x = 0;
	int32_t z = 0;
	for (i = 0; i < 100000ULL; i++) {
		currentSeed = rndSeed++;
		setMCSeed(&seed, currentSeed);

		for (chunkX = xMin; chunkX < xMax; chunkX += 2) {
			for (chunkZ = zMin; chunkZ < zMax; chunkZ += 2) {
				if (isSlimeChunkXZ(&seed, chunkX + 2, chunkZ) && isSlimeChunkXZ(&seed, chunkX + 2, chunkZ + 2)) {
					if (isSlimeChunkXZ(&seed, chunkX, chunkZ) && isSlimeChunkXZ(&seed, chunkX, chunkZ + 2)) {
						for (z = -1; z < 1; z++) {
							for (x = -1; x < 1; x++) {
								slimeChunkCount = 4;
								chunkCount = 4;
								for (exX = 0; exX < countRangeX; exX++) {
									for (exZ = 0; exZ < countRangeZ; exZ++) {
										chunkCount++;
										if ((exX + x) % 2 != 0 || (exZ + z) % 2 != 0) {
											if (isSlimeChunkXZ(&seed, chunkX + exX, chunkZ + exZ)) {
												slimeChunkCount++;
											}
										}
									}
								}
								if (slimeChunkCount >= minSlimeChunks) {
									printf("'%"PRId64",%"PRId32",%"PRId32",%"PRId32",%"PRId32"\n", currentSeed, chunkX * 16, chunkZ * 16, slimeChunkCount, chunkCount);
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
	return;
}
