
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
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

  int64_t x, xMin = -313, xMax = 312;
  int64_t z, zMin = -313, zMax = 312;
  int64_t countRangeX = 4;
  int64_t countRangeZ = 4;
  int64_t minSlimeChunks = 14;
  uint64_t i = 0;
  int32_t slimeChunkCount = 0;
  int32_t chunkCount = 0;
  long exX = 0;
  long exZ = 0;
  for(i = 0; i < 100000ULL; i++){
    currentSeed = rndSeed++;
    setMCSeed(&seed, currentSeed);

    for(x = xMin; x < xMax; x++){
      for(z = zMin; z < zMax; z++){
        if(isSlimeChunkXZ(&seed, x, z)){
          slimeChunkCount = 0;
          chunkCount = 0;
          for(exX = 0; exX < countRangeX; exX++) {
            for(exZ = 0; exZ < countRangeZ; exZ++) {
              chunkCount++;
              if(isSlimeChunkXZ(&seed, x + exX, z + exZ)){
                slimeChunkCount++;
              }
            }
          }
          if(slimeChunkCount >= minSlimeChunks){
            printf("'%"PRId64",%"PRId64",%"PRId64",%"PRId32",%"PRId32"\n", currentSeed, x * 16, z * 16, slimeChunkCount, chunkCount);
          }
        }
      }
    }
  }
}

