#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{ 
	TEST_CLASS(PKBTest)
	{
	public:

		TEST_METHOD(PKBTableInsertTest)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			int tableValuesCount;

			for (int i = 1; i < 8; i++) {
				switch (i) {
				case 1:
					tableValuesCount = 4;
					break;
				case 2:
					tableValuesCount = 5;
					break;
				case 3:
					tableValuesCount = 2;
					break;
				case 4:
					tableValuesCount = 2;
					break;
				case 5:
					tableValuesCount = 2;
					break;
				case 6:
					tableValuesCount = 2;
					break;
				case 7:
					tableValuesCount = 1;
					break;
				}
				data.clear();
				for (int j = 0; j < tableValuesCount; j++) {
					data.push_back({ 1 });
				}
				pkb.insertToTable(i, 1, data);
				Assert::AreEqual(tableValuesCount, static_cast<int>(pkb.getFromTable(i, 1).size()));
			}
		}

		TEST_METHOD(PKBNameTableInsertTest)
		{
			PKB pkb;

			string data = "x";

			for (int i = 8; i < 10; i++) {
				pkb.insertToNameTable(i, data);
				Assert::AreEqual(string("x"), pkb.getFromNameTable(i, 1));
			}
		}

		TEST_METHOD(PKBProcedureMethods)
		{
			PKB pkb;

			string data = { "proc_x" };

			int return_id;

			return_id = pkb.insertToNameTable(8, data);
			Assert::AreEqual(string("proc_x"), pkb.getProcedureName(return_id));
		}

		TEST_METHOD(PKBVariableMethods)
		{
			PKB pkb;

			string data = { "x" };

			int return_id;

			return_id = pkb.insertToNameTable(9, data);
			Assert::AreEqual(string("x"), pkb.getVariableName(return_id));
		}

		TEST_METHOD(PKBFollows)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { {1}, {}, {}, {1} });
			pkb.insertToTable(1, 2, { {1}, {}, {}, {1} });
			pkb.insertToTable(2, 1, { {0}, {1, 2}, {}, {}, {1} });

			Assert::AreEqual(1, pkb.getFollowsBefore(2));
			Assert::AreEqual(2, pkb.getFollowsAfter(1));
		}

		TEST_METHOD(PKBFollowsStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{},{},{ 1 } });

			std::vector<int> dataBeforeStar = {1, 2};
			std::vector<int> dataAfterStar = {2, 3};
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == dataBeforeStar));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == dataAfterStar));
		}

		TEST_METHOD(PKBCheckFollows)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkFollows(1, 2));
		}

		TEST_METHOD(PKBCheckFollowsStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkFollowsStar(1, 3));
		}

		TEST_METHOD(PKBGetAllFollows)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{},{},{ 1 } });

			Assert::AreEqual(3, static_cast<int>(pkb.getAllFollows().size()));
		}

		TEST_METHOD(PKBGetAllFollowsStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{},{},{ 1 } });
			
			unordered_map<int, std::vector<int>> follows = pkb.getAllFollowsStar();
			int size = 0;
			for (auto it = follows.begin(); it != follows.end(); ++it) {
				size += it->second.size();
			}
			Assert::AreEqual(6, size);
		}
	};

}