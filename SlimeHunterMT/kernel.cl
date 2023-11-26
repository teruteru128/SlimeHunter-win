
typedef unsigned long long uint64_t;

__kernel void tallySlimeCunks(__global int *result, __global uint64_t *chunks) {
  size_t x = get_global_id(0);
  size_t z = get_global_id(1);
  size_t index = z * 622 + x;
  result[index] = 0;
  result[index] += (chunks[(((z + 0) * 625) + (x + 0)) >> 6] >> ((((z + 0) * 625) + (x + 0)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 0) * 625) + (x + 1)) >> 6] >> ((((z + 0) * 625) + (x + 1)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 0) * 625) + (x + 2)) >> 6] >> ((((z + 0) * 625) + (x + 2)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 0) * 625) + (x + 3)) >> 6] >> ((((z + 0) * 625) + (x + 3)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 1) * 625) + (x + 0)) >> 6] >> ((((z + 1) * 625) + (x + 0)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 1) * 625) + (x + 1)) >> 6] >> ((((z + 1) * 625) + (x + 1)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 1) * 625) + (x + 2)) >> 6] >> ((((z + 1) * 625) + (x + 2)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 1) * 625) + (x + 3)) >> 6] >> ((((z + 1) * 625) + (x + 3)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 2) * 625) + (x + 0)) >> 6] >> ((((z + 2) * 625) + (x + 0)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 2) * 625) + (x + 1)) >> 6] >> ((((z + 2) * 625) + (x + 1)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 2) * 625) + (x + 2)) >> 6] >> ((((z + 2) * 625) + (x + 2)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 2) * 625) + (x + 3)) >> 6] >> ((((z + 2) * 625) + (x + 3)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 3) * 625) + (x + 0)) >> 6] >> ((((z + 3) * 625) + (x + 0)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 3) * 625) + (x + 1)) >> 6] >> ((((z + 3) * 625) + (x + 1)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 3) * 625) + (x + 2)) >> 6] >> ((((z + 3) * 625) + (x + 2)) & 0x3f)) & 0x01;
  result[index] += (chunks[(((z + 3) * 625) + (x + 3)) >> 6] >> ((((z + 3) * 625) + (x + 3)) & 0x3f)) & 0x01;
}
