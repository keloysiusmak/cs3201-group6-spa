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
			std::vector<int> innerdata;
			innerdata.push_back(1);
			std::vector<std::vector<int>> data;
			data.push_back(innerdata);
			data.push_back(innerdata);
			data.push_back(innerdata);
			data.push_back(innerdata);

			pkb.insertToTable(1, 1, data);

			unordered_map<int, std::vector<std::vector<int>>> table = pkb.tables[1];
			data = table[1];
			std::vector<std::vector<int>> data2;

			Assert::AreEqual(1, static_cast<int>(pkb.tables[1].size()));

		}

	};
}