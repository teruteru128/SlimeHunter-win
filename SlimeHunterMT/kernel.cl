
typedef unsigned long uint64_t;
typedef long int64_t;
typedef int int32_t;

static inline int isSlimeChunk(uint64_t seed, int chunkX, int chunkZ);

__kernel void tallySlimeCunks(__global char *result, __global uint64_t *seed) {
  size_t id0 = get_global_id(0);
  size_t id1 = get_global_id(1);
  size_t id2 = get_global_id(2);
  // size_t size0 = get_global_size(0);
  size_t size1 = get_global_size(1);
  size_t size2 = get_global_size(2);
  size_t index = (id2 * size2 + id1) * size1 + id0;
  int val = 0;
  // seed + index ?
  uint64_t worldSeed = *seed + index;
  result[index] = 0;
}

static inline void setSeed(uint64_t *seed, int64_t value) {
  *seed = (value ^ 0x5deece66d) & ((1ULL << 48) - 1);
}
static inline int32_t next(uint64_t *seed, const int bits) {
  *seed = (*seed * 0x5deece66d + 0xb) & ((1ULL << 48) - 1);
  return (int)((int64_t)*seed >> (48 - bits));
}
static inline int32_t nextInt(uint64_t *seed, const int32_t n) {
  int bits, val;
  const int m = n - 1;

  if ((m & n) == 0) {
    uint64_t x = n * (uint64_t)next(seed, 31);
    return (int)((int64_t)x >> 31);
  }

  do {
    bits = next(seed, 31);
    val = bits % n;
  } while (bits - val + m < 0);
  return val;
}
static inline int isSlimeChunk(uint64_t seed, int chunkX, int chunkZ) {
  uint64_t rnd = seed;
  rnd += (int)(chunkX * 0x5ac0db);
  rnd += (int)(chunkX * chunkX * 0x4c1906);
  rnd += (int)(chunkZ * 0x5f24f);
  rnd += (int)(chunkZ * chunkZ) * 0x4307a7ULL;
  rnd ^= 0x3ad8025fULL;
  setSeed(&rnd, rnd);
  return nextInt(&rnd, 10) == 0;
}
