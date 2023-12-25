
typedef unsigned long uint64_t;
typedef long int64_t;
typedef int int32_t;

static inline int isSlimeChunk(uint64_t seed, int chunkX, int chunkZ);

__kernel void tallySlimeCunks(__global char *result, __global const uint64_t *seed) {
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
  result[0] = isSlimeChunk(worldSeed, -202, -49);
  // 1スレッドで実行するコードが増えるとバグるんだな、多分
}
static int isSlimeChunk(const uint64_t seed, int chunkX, int chunkZ) {
  uint64_t rnd = seed;
  rnd += (int)(chunkX * 0x5ac0db);
  rnd += (int)(chunkX * chunkX * 0x4c1906);
  rnd += (int)(chunkZ * 0x5f24f);
  rnd += (int)(chunkZ * chunkZ) * 0x4307a7UL;
  rnd ^= 0x5E434E432UL;
  rnd &= 0xffffffffffffUL;
  int bits, val;
  const int m = 9;
  do {
    rnd = (rnd * 0x5deece66dUL + 0xb) & 0xffffffffffffUL;
    bits = (int)(rnd >> 17);
    val = bits % 10;
  } while (bits - val + m < 0);
  return val == 0;
}
