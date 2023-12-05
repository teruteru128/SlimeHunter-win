
#include "pch.h"
#include "finders.h"
#include "search.hpp"
#include <bitset>
#include <iostream>

using std::bitset;

int mpsample(void) {
	bitset<625 * 625>* set = new bitset<625 * 625>();
	int pos = 0;
	for (pos = 0; pos < 390625; pos++) {
		set->set(pos, isSlimeChunk(263622805221400ULL, (pos % 625) - 312, (pos / 625) - 312));
	}
	size_t tempZ0 = 0;
	size_t tempZ0_r = 0;
	int x = 0;
	bool found = false;
	for (tempZ0 = 0; tempZ0 < 388125; tempZ0 += 625) {
		tempZ0_r = 388125 - tempZ0;
#pragma omp parallel for reduction(| : found)
		for (x = 621; x >= 0; x--) {
			found |= extracted(set, tempZ0 + x);
		}
	}
	std::cout << found << std::endl;
	return 0;
}
