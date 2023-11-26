
typedef unsigned long long uint64_t;

static inline size_t pos(size_t x, size_t z) { return z * 625 + x; }

__kernel void tallySlimeCunks(__global int *result, __global uint64_t *chunks) {
  size_t x = get_global_id(0);
  size_t z = get_global_id(1);
  size_t index = z * 622 + x;
  result[index] = (chunks[pos(x + 0, z + 0) >> 6] >> (pos(x + 0, z + 0) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 1, z + 0) >> 6] >> (pos(x + 1, z + 0) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 2, z + 0) >> 6] >> (pos(x + 2, z + 0) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 3, z + 0) >> 6] >> (pos(x + 3, z + 0) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 0, z + 1) >> 6] >> (pos(x + 0, z + 1) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 1, z + 1) >> 6] >> (pos(x + 1, z + 1) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 2, z + 1) >> 6] >> (pos(x + 2, z + 1) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 3, z + 1) >> 6] >> (pos(x + 3, z + 1) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 0, z + 2) >> 6] >> (pos(x + 0, z + 2) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 1, z + 2) >> 6] >> (pos(x + 1, z + 2) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 2, z + 2) >> 6] >> (pos(x + 2, z + 2) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 3, z + 2) >> 6] >> (pos(x + 3, z + 2) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 0, z + 3) >> 6] >> (pos(x + 0, z + 3) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 1, z + 3) >> 6] >> (pos(x + 1, z + 3) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 2, z + 3) >> 6] >> (pos(x + 2, z + 3) & 0x3f)) & 0x01;
  result[index] += (chunks[pos(x + 3, z + 3) >> 6] >> (pos(x + 3, z + 3) & 0x3f)) & 0x01;
}
