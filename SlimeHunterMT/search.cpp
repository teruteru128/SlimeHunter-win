
#define CL_TARGET_OPENCL_VERSION 300
#include "mcSlimeChunkOracle.h"
#include "pch.h"
#include "rnd.h"
#include "search.hpp"
#include <algorithm>
#include <atomic>
#include <bitset>
#include <CL/cl.h>
#include <inttypes.h>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
using std::bitset;

static bool extracted(bitset<625 * 625>* set, int x, int z);

volatile std::atomic_int cont = 1;
volatile std::atomic_uint64_t seed;

Result* task(Config* config) {
	auto set = new bitset<625 * 625>();
	int lineCombo = 0;
	int lineComboMax = 1;
	int x = 0;
	int z = 0;
	int tempZ0;
	int tempZ1;
	uint64_t worldSeed = 0;
	bool found = false;
	while (cont) {
		worldSeed = seed++;
		for (z = 0, tempZ0 = 0; z < 625; z++, tempZ0 += 625) {
			for (x = 0; x < 625; x++) {
				set->set(tempZ0 + x, isSlimeChunk(worldSeed, x - 312, z - 312));
			}
		}
		found = false;
		for (z = 621, tempZ0 = 388125; z >= 0; z--, tempZ0 -= 625) {
			lineCombo = 0;
			lineComboMax = 1;
			// ���ԂƋ�Ԃ̃g���[�h�I�t
			// �A���ŃX���C���`�����N������ł���𐔂���
			// �r�b�g���Z�ŃX���C���`�����N��4�A�����Ă��邩�����Ԃ�
			for (x = 0; lineComboMax && x < 625; x++) {
				lineCombo = ((lineCombo << 1) | (int)set->test(tempZ0 + x)) & 15;
				lineComboMax &= lineCombo != 15;
			}
			// 4�����Ȃ�X�L�b�v
			if (lineComboMax)
			{
				z -= 3;
				tempZ0 -= 1875;
				continue;
			}
			tempZ1 = tempZ0 + 625;
			lineCombo = 0;
			lineComboMax = 1;
			for (x = 0; lineComboMax && x < 625; x++) {
				lineCombo = ((lineCombo << 1) | (int)set->test(tempZ1 + x)) & 15;
				lineComboMax &= lineCombo != 15;
			}
			// 4�����Ȃ�X�L�b�v
			if (lineComboMax)
			{
				z -= 2;
				tempZ0 -= 1250;
				continue;
			}
			for (x = 621; x >= 0; x--)
			{
				if (extracted(set, x, z)) {
					cont = 0;
					std::cout << "�������[�I[" << worldSeed << "]" << std::endl;
					// delete set;
					return new Result(worldSeed, x - 312, z - 312);
				}
			}
		}
	}

	return NULL;
}

// TODO &&�Ōq�����Z���]����&�Ōq�������@�I���s�łǂ����̂ق��������Ȃ�񂾂낤��? -> �x���`�}�[�N
// TODO �x���`�}�[�N -> ����̃V�[�h�ł͂Ȃ������_���ȃV�[�h���g���Čv������Ȃ���΂Ȃ�Ȃ�
// TODO �S�X���b�h��1�V�[�h���������ׂ��Ȃ̂��A�e�X���b�h�ł��ꂼ��ʂ̃V�[�h��T�����ׂ��Ȃ̂�(�v�Z���x�̖��)
static inline bool extracted(bitset<625 * 625>* set, int x, int z) {
	return set->test((z + 3) * 625 + x + 0) && set->test((z + 3) * 625 + x + 1) && set->test((z + 3) * 625 + x + 2) && set->test((z + 3) * 625 + x + 3) &&
		set->test((z + 2) * 625 + x + 0) && set->test((z + 2) * 625 + x + 1) && set->test((z + 2) * 625 + x + 2) && set->test((z + 2) * 625 + x + 3) &&
		set->test((z + 1) * 625 + x + 0) && set->test((z + 1) * 625 + x + 1) && set->test((z + 1) * 625 + x + 2) && set->test((z + 1) * 625 + x + 3) &&
		set->test((z + 0) * 625 + x + 0) && set->test((z + 0) * 625 + x + 1) && set->test((z + 0) * 625 + x + 2) && set->test((z + 0) * 625 + x + 3);
}

int mpsample(void) {
	bitset<625 * 625>* set = new bitset<625 * 625>();
	int pos = 0;
	for (pos = 0; pos < 390625; pos++) {
		set->set(pos, isSlimeChunk(263622805221400ULL, (pos % 625) - 312, (pos / 625) - 312));
	}
	int z = 0;
	int x = 0;
	bool found = false;
	for (z = 621; z >= 0; z--) {
#pragma omp parallel for reduction(| : found)
		for (x = 621; x >= 0; x--) {
			found |= extracted(set, x, z);
		}
	}
	std::cout << found << std::endl;
	return 0;
}

Config::Config() {
}
Config::~Config() {}

Result::Result(uint64_t worldSeed, int x, int z)
{
	this->worldSeed = worldSeed;
	this->x = x;
	this->z = z;
}

Result::~Result()
{
}

uint64_t Result::getWorldSeed() {
	return this->worldSeed;
}

int Result::getX() {
	return this->x;
}

int Result::getZ() {
	return this->z;
}
