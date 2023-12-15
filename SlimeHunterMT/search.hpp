
#pragma once
#ifndef SLIME_HUNTER_H
#define SLIME_HUNTER_H
#include "finders.h"
#include "rnd.h"
#include <atomic>
#include <bitset>
#include <stdint.h>

using std::bitset;

class Config {
public:
	Config(std::atomic_uint64_t*);
	~Config();
	std::atomic_uint64_t* getSeed();
protected:
private:
	std::atomic_uint64_t* seed;
};
class Result
{
public:
	Result(uint64_t worldSeed, int x, int z);
	~Result();
	uint64_t getWorldSeed() const;
	int getX() const;
	int getZ() const;
private:
	uint64_t worldSeed;
	int x;
	int z;
};


// TODO &&�Ōq�����Z���]����&�Ōq�������@�I���s�łǂ����̂ق��������Ȃ�񂾂낤��? -> �x���`�}�[�N
// TODO �x���`�}�[�N -> ����̃V�[�h�ł͂Ȃ������_���ȃV�[�h���g���Čv������Ȃ���΂Ȃ�Ȃ�
// TODO �S�X���b�h��1�V�[�h���������ׂ��Ȃ̂��A�e�X���b�h�ł��ꂼ��ʂ̃V�[�h��T�����ׂ��Ȃ̂�(�v�Z���x�̖��)
// pos->�k��, pos+3->�k��, pos+1875->�쐼, pos+1878->�쓌
// �Ƃ������|���Z�̏������炢�R���p�C�����ł���Ă����̂�
inline bool extracted(bitset<625 * 625>* set, size_t pos) {
	return set->test(pos + 1875) && set->test(pos + 1876) && set->test(pos + 1877) && set->test(pos + 1878) &&
		set->test(pos + 1250) && set->test(pos + 1251) && set->test(pos + 1252) && set->test(pos + 1253) &&
		set->test(pos + 625) && set->test(pos + 626) && set->test(pos + 627) && set->test(pos + 628) &&
		set->test(pos) && set->test(pos + 1) && set->test(pos + 2) && set->test(pos + 3);
}

Result* task(Config* config);
extern volatile std::atomic_int cont;

#endif
