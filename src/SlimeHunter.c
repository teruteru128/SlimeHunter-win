
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "random.h"
#include "mcSlimeChunkOracle.h"

int main(int argc, char* argv[]){
  SlimeChunkSeed seed;
  Random random;
  setSeed(&random, 0);
  printf("%u\n", next(&random, 32));
  printf("%u\n", nextInt(&random));
  printf("%u\n", nextIntWithRange(&random, 10));
  printf("%"PRId64"\n", nextLong(&random));

  FILE* fp;
  char* rpath = "/dev/urandom";
  fp = fopen(rpath,"rb");
  if(fp == NULL){
    return EXIT_FAILURE;
  }
  int64_t rndSeed = 0;
  int64_t currentSeed = 0;
  (void)fread(&rndSeed, sizeof(rndSeed), 1, fp);

  printf("Initial Seed : %"PRId64"\n", rndSeed);

  int64_t chunkX, xMin = -313, xMax = 312, x;
  int64_t chunkZ, zMin = -313, zMax = 312, z;
  int64_t countRangeX = 4;
  int64_t countRangeZ = 4;
  int64_t minSlimeChunks = 13;
  uint64_t i = 0;
  int32_t slimeChunkCount = 0;
  int32_t chunkCount = 0;
  long exX = 0;
  long exZ = 0;
  clock_t start = clock();
  int64_t searchSeeds = 10000ULL;
  for(i = 0; i < searchSeeds; i++){
    currentSeed = rndSeed++;
    setMCSeed(&seed, currentSeed);

    for(chunkZ = zMin; chunkZ < zMax; chunkZ += 2){
      for(chunkX = xMin; chunkX < xMax; chunkX += 2){
        if(isSlimeChunkXZ(&seed, chunkX + 2, chunkZ) && isSlimeChunkXZ(&seed, chunkX + 2, chunkZ + 2)){
          if(isSlimeChunkXZ(&seed, chunkX, chunkZ) && isSlimeChunkXZ(&seed, chunkX, chunkZ + 2)){
            for(z = -1; z < 1; z ++){
              for(x = -1; x < 1; x++){
                slimeChunkCount = 4;
                chunkCount = 4;
                for(exX = 0; exX < countRangeX; exX++) {
                  for(exZ = 0; exZ < countRangeZ; exZ++) {
                    if((z + exZ) % 2 != 0 || (x + exX) % 2 != 0){
                      chunkCount++;
                      if(isSlimeChunkXZ(&seed, chunkX + exX + x, chunkZ + exZ + z)){
                        slimeChunkCount++;
                      }
                    }
                  }
                }
                if(slimeChunkCount >= minSlimeChunks){
                  printf("'%"PRId64",(%"PRId64",%"PRId64")(%"PRId64",%"PRId64"),(%"PRId32",%"PRId32")\n", currentSeed, chunkX, chunkZ, chunkX * 16, chunkZ * 16, slimeChunkCount, chunkCount);
                }
              }
            }
          }
        } else {
          chunkX += 2;
        }
      }
    }
  }
  clock_t finish = clock();
  double seconds = ((double)(finish - start)/ CLOCKS_PER_SEC);
  printf("%"PRId64"seeds, %.2lfseeds/s, %.2lfs\n", searchSeeds, searchSeeds/seconds, seconds);
}

