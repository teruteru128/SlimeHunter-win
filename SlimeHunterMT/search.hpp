
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


// TODO &&で繋いだ短絡評価と&で繋いだ投機的実行でどっちのほうが早くなるんだろうか? -> ベンチマーク
// TODO ベンチマーク -> 特定のシードではなくランダムなシードを使って計測されなければならない
// TODO 全スレッドで1シードを検査すべきなのか、各スレッドでそれぞれ別のシードを探索すべきなのか(計算粒度の問題)
// pos->北西, pos+3->北東, pos+1875->南西, pos+1878->南東
// というか掛け算の除去ぐらいコンパイラ側でやってくれるのね
inline bool extracted(bitset<625 * 625>* set, size_t pos) {
	return set->test(pos + 1875) && set->test(pos + 1876) && set->test(pos + 1877) && set->test(pos + 1878) &&
		set->test(pos + 1250) && set->test(pos + 1251) && set->test(pos + 1252) && set->test(pos + 1253) &&
		set->test(pos + 625) && set->test(pos + 626) && set->test(pos + 627) && set->test(pos + 628) &&
		set->test(pos) && set->test(pos + 1) && set->test(pos + 2) && set->test(pos + 3);
}

Result* task(Config* config);
extern volatile std::atomic_int cont;

#endif
