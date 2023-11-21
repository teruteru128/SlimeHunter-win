#include "pch.h"
#include "CppUnitTest.h"
#include "../SlimeHunterMT/rnd.h"
#include "../SlimeHunterMT/slimeHunter.hpp"

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
	};
}
