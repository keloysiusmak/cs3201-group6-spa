#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PKBTest)
	{
	public:

		TEST_METHOD(PKBInsertTest)
		{
			PKB pkb;
			std::vector<std::vector<int>> data = { {1}, {1}, {1}, {1} };
			
			Assert::AreEqual(true, pkb.insertToTable(1, 1, data));

			data = pkb.getFromTable(1, 1);

			//Assert::AreEqual(1, data[0][0]);

		}

	};
}