#include "pch.h"
#include "CppUnitTest.h"
#include "rnd.h"
#include "slimeHunter.hpp"
#include <bitset>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::IsTrue(isSlimeChunk(263622805221400L, 129, -48));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 130, -48));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 131, -48));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 132, -48));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 129, -47));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 130, -47));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 131, -47));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 132, -47));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 129, -46));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 130, -46));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 131, -46));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 132, -46));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 129, -45));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 130, -45));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 131, -45));
			Assert::IsTrue(isSlimeChunk(263622805221400L, 132, -45));
		}

		TEST_METHOD(TestMethod2) {
			using std::bitset;
			bitset<625 * 625>* set = new bitset<625 * 625>();
			int z = 0;
			int x = 0;
			for (z = 0; z < 625; z++) {
				for (x = 0; x < 625; x++) {
					set->set(z * 625 + x, (bool)isSlimeChunk(263622805221400L, x - 312, z - 312));
				}
			}
			Assert::IsTrue(set->test((-48 + 312) * 625 + (129 + 312)));
			Assert::IsTrue(set->test((-48 + 312) * 625 + (130 + 312)));
			Assert::IsTrue(set->test((-48 + 312) * 625 + (131 + 312)));
			Assert::IsTrue(set->test((-48 + 312) * 625 + (132 + 312)));
			Assert::IsTrue(set->test((-47 + 312) * 625 + (129 + 312)));
			Assert::IsTrue(set->test((-47 + 312) * 625 + (130 + 312)));
			Assert::IsTrue(set->test((-47 + 312) * 625 + (131 + 312)));
			Assert::IsTrue(set->test((-47 + 312) * 625 + (132 + 312)));
			Assert::IsTrue(set->test((-46 + 312) * 625 + (129 + 312)));
			Assert::IsTrue(set->test((-46 + 312) * 625 + (130 + 312)));
			Assert::IsTrue(set->test((-46 + 312) * 625 + (131 + 312)));
			Assert::IsTrue(set->test((-46 + 312) * 625 + (132 + 312)));
			Assert::IsTrue(set->test((-45 + 312) * 625 + (129 + 312)));
			Assert::IsTrue(set->test((-45 + 312) * 625 + (130 + 312)));
			Assert::IsTrue(set->test((-45 + 312) * 625 + (131 + 312)));
			Assert::IsTrue(set->test((-45 + 312) * 625 + (132 + 312)));
			delete set;
		}
	};
}
