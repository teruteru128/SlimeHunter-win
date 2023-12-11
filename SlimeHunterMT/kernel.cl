
typedef unsigned long uint64_t;
typedef long int64_t;
typedef int int32_t;

static inline void setSeed(uint64_t *seed, int64_t value);
static inline int32_t next(uint64_t *seed, const int bits);
static inline int32_t nextInt(uint64_t *seed, const int32_t n);
static inline int isSlimeChunk(uint64_t seed, int chunkX, int chunkZ);
static inline size_t pos(size_t x, size_t z) { return z * 625 + x; }
static inline int unitIndex(int bitIndex) { return bitIndex >> 6; }
static inline int bit(int bitIndex) {
  return 1L << (bitIndex & ((1 << 6) - 1));
}

static inline void setValue(uint64_t *map, size_t bitIndex, int val) {
  if (val) {
    map[unitIndex(bitIndex)] |= bit(bitIndex);
  } else {
    map[unitIndex(bitIndex)] &= ~bit(bitIndex);
  }
}
static inline int getValue(uint64_t *map, size_t bitIndex) {
  return ((map[unitIndex(bitIndex)] & bit(bitIndex)) != 0);
}

__kernel void tallySlimeCunks(__global char *result, __global uint64_t *seed) {
  uint64_t map[6104];
  int x;
  int z;
  size_t tempZ0 = 0;
  size_t pos;
  size_t wordIndex = 0;
  size_t shift = 0;
  size_t id0 = get_global_id(0);
  size_t id1 = get_global_id(1);
  size_t id2 = get_global_id(2);
  //size_t size0 = get_global_size(0);
  size_t size1 = get_global_size(1);
  size_t size2 = get_global_size(2);
  size_t index = (id2 * size2 + id1) * size1 + id0;
  int val = 0;
  result[index] = 0;
  // seed + index ?
  uint64_t worldSeed = *seed + index;
  // 南４行を埋める
  for (z = 621, tempZ0 = 388125; z < 625; z++, tempZ0 += 625) {
    for (x = 0; x < 625; x++) {
      pos = tempZ0 + x;
      setValue(map, pos, isSlimeChunk(worldSeed, x - 312, z - 312));
    }
  }
  // 東４列を埋める
  for (z = 0, tempZ0 = 0; z < 625; z++, tempZ0 += 625) {
    for (x = 621; x < 625; x++) {
      pos = tempZ0 + x;
      setValue(map, pos, isSlimeChunk(worldSeed, x - 312, z - 312));
    }
  }
  // 南東隅からチェックする
  for (z = 620, tempZ0 = 387500; z >= 0; z--, tempZ0 -= 625) {
    for (x = 620; x >= 0; x--) {
      pos = tempZ0 + x;
      val = isSlimeChunk(worldSeed, x - 312, z - 312);
      setValue(map, pos, val);
      result[index] |= val & getValue(map, pos + 1) & getValue(map, pos + 2) & getValue(map, pos + 3) & getValue(map, pos + 4) &
                       getValue(map, pos + 625) & getValue(map, pos + 626) & getValue(map, pos + 627) & getValue(map, pos + 628) & getValue(map, pos + 629) &
                       getValue(map, pos + 1250) & getValue(map, pos + 1251) & getValue(map, pos + 1252) & getValue(map, pos + 1253) & getValue(map, pos + 1254) &
                       getValue(map, pos + 1875) & getValue(map, pos + 1876) & getValue(map, pos + 1877) & getValue(map, pos + 1878) & getValue(map, pos + 1879) &
                       getValue(map, pos + 2500) & getValue(map, pos + 2501) & getValue(map, pos + 2502) & getValue(map, pos + 2503) & getValue(map, pos + 2504);
    }
  }
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
