#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB.h"
#include "../source/PatternObject.h"

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
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));
			
			int tableValuesCount;

			for (int i = 1; i < 8; i++) {
				switch (i) {
				case 1:
					tableValuesCount = 4;
					break;
				case 2:
					tableValuesCount = 3;
					break;
				case 3:
					tableValuesCount = 3;
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
				Assert::AreEqual(true, (pkb.getFromTable(i, 1) == data));
			}
		}


		TEST_METHOD(PKBInvalidTableInsertTest)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			int tableValuesCount;

			for (int i = 1; i < 8; i++) {
				switch (i) {
				case 1:
					tableValuesCount = 4;
					break;
				case 2:
					tableValuesCount = 3;
					break;
				case 3:
					tableValuesCount = 3;
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
				for (int j = 0; j <= tableValuesCount; j++) {
					data.push_back({ 1 });
				}
				pkb.insertToTable(i, 1, data);
				Assert::AreEqual(0, static_cast<int>(pkb.getFromTable(i, 1).size()));
				Assert::AreEqual(false, (pkb.getFromTable(i, 1) == data));
			}
		}

		TEST_METHOD(PKBMultipleTableInsertTest)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { {1}, {}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			data = { {},{1},{},{} };
			std::vector<std::vector<int>> dataFinal = { {1}, {1}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));

			data = { {},{ 2 },{},{} };
			dataFinal = { { 1 },{ 1, 2 },{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBMultipleTableInsertTestSameValue)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { { 1 },{},{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			data = { {},{ 1 },{},{} };
			pkb.insertToTable(1, 1, data);

			data = { {},{ 1 },{},{} };
			std::vector<std::vector<int>> dataFinal = { { 1 },{ 1 },{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBInvalidMultipleTableInsertTest)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { {1}, {}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));
			data = { {},{1},{},{} };
			std::vector<std::vector<int>> dataFinal = { {1}, {1}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBNameTableInsertTest)
		{
			PKB pkb;

			string data = "x";

			for (int i = 8; i < 10; i++) {
				/* Null Test */
				Assert::AreEqual(string(""), pkb.getFromNameTable(i, 1));

				Assert::AreEqual(string("x"), pkb.getFromNameTable(i, pkb.insertToNameTable(i, data)));
			}
		}

		TEST_METHOD(PKBNameTableAlreadyInsertedTest)
		{
			PKB pkb;

			string data = "x";

			for (int i = 8; i < 10; i++) {
				/* Null Test */
				Assert::AreEqual(string(""), pkb.getFromNameTable(i, 1));

				Assert::AreEqual(0, static_cast<int>(pkb.nameTables[i - 8].size()));
				Assert::AreEqual(1, pkb.insertToNameTable(i, data));
				Assert::AreEqual(1, static_cast<int>(pkb.nameTables[i - 8].size()));
				Assert::AreEqual(1, pkb.insertToNameTable(i, data));
				Assert::AreEqual(1, static_cast<int>(pkb.nameTables[i - 8].size()));
			}
		}

		TEST_METHOD(PKBProcedureMethods)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(string(""), pkb.getFromNameTable(8, 1));

			string data = "proc_x" ;

			int return_id;

			return_id = pkb.insertToNameTable(8, data);
			Assert::AreEqual(string("proc_x"), pkb.getProcedureName(return_id));
		}

		TEST_METHOD(PKBVariableMethods)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(string(""), pkb.getFromNameTable(9, 1));

			string data = "x";

			int return_id;

			return_id = pkb.insertToNameTable(9, data);
			Assert::AreEqual(string("x"), pkb.getVariableName(return_id));
		}

		TEST_METHOD(PKBGetAllVariables)
		{
			PKB pkb;
			std::vector<int> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllVariables() == data));

			data.push_back(pkb.insertToNameTable(9, "a"));
			data.push_back(pkb.insertToNameTable(9, "b"));

			Assert::AreEqual(true, (pkb.getAllVariables() == data));
		}

		TEST_METHOD(PKBGetAllProcedures)
		{
			PKB pkb;
			std::vector<int> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllProcedures() == data));

			data.push_back(pkb.insertToNameTable(8, "a"));
			data.push_back(pkb.insertToNameTable(8, "b"));

			Assert::AreEqual(true, (pkb.getAllProcedures() == data));
		}

		TEST_METHOD(PKBGetAllStatements)
		{
			PKB pkb;

			std::vector<int> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatements() == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			data = { 1, 2, 3 };

			Assert::AreEqual(true, (pkb.getAllStatements() == data));
		}

		TEST_METHOD(PKBGetAllStatementsWithType)
		{
			PKB pkb;

			std::vector<int> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 2 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 3 } });

			data = { 1, 3 };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			data = { 2 };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(2) == data));
		}

		TEST_METHOD(PKBFollows)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(0, pkb.getFollowsBefore(2));
			Assert::AreEqual(0, pkb.getFollowsAfter(1));

			pkb.insertToTable(1, 1, { {1}, {}, {}, {1} });
			pkb.insertToTable(1, 2, { {1}, {}, {}, {1} });

			/* Invalid Tests */
			Assert::AreEqual(0, pkb.getFollowsBefore(2));
			Assert::AreEqual(0, pkb.getFollowsAfter(1));

			pkb.insertToTable(2, 1, { {0}, {1, 2}, {1} });

			Assert::AreEqual(1, pkb.getFollowsBefore(2));
			Assert::AreEqual(2, pkb.getFollowsAfter(1));
			Assert::AreEqual(0, pkb.getFollowsAfter(2));
			Assert::AreEqual(0, pkb.getFollowsBefore(0));
		}

		TEST_METHOD(PKBFollowsStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<int> data;
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{ 1 } });

			std::vector<int> dataBeforeStar = {1, 2};
			std::vector<int> dataAfterStar = {2, 3};
			std::vector<int> nullData;
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(1) == nullData));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(3) == nullData));
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == dataBeforeStar));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == dataAfterStar));
		}

		TEST_METHOD(PKBCheckFollows)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 2));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 2));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 3 },{ 1 } });

			Assert::AreEqual(true, pkb.checkFollows(1, 2));
			Assert::AreEqual(false, pkb.checkFollows(2, 1));
			Assert::AreEqual(false, pkb.checkFollows(1, 3));
			Assert::AreEqual(false, pkb.checkFollows(3, 1));
		}

		TEST_METHOD(PKBCheckFollowsStar)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 4 },{ 1 } });

			Assert::AreEqual(true, pkb.checkFollowsStar(1, 3));
			Assert::AreEqual(true, pkb.checkFollowsStar(2, 3));
			Assert::AreEqual(false, pkb.checkFollowsStar(3, 1));
			Assert::AreEqual(false, pkb.checkFollowsStar(3, 2));
			Assert::AreEqual(false, pkb.checkFollowsStar(1, 4));
			Assert::AreEqual(false, pkb.checkFollowsStar(4, 1));
		}

		TEST_METHOD(PKBGetAllFollows)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollows().size()));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 5, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollows().size()));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 5 },{ 1 } });

			std::vector<std::vector<int>> data = { {1,2}, {2,3}, {3,4} };
			Assert::AreEqual(true, (pkb.getAllFollows() == data));
		}

		TEST_METHOD(PKBGetAllFollowsStar)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollowsStar().size()));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollowsStar().size()));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{},{},{ 1 } });
			
			unordered_map<int, std::vector<int>> follows = pkb.getAllFollowsStar();
			std::vector<std::vector<int>> data = { {2,3,4}, {3,4}, {4} };
			int i = 0;
			for (auto it = follows.begin(); it != follows.end(); ++it) {
				Assert::AreEqual(true, (it->second == data[i]));
				i++;
			}

		}

		TEST_METHOD(PKBParent)
		{
			PKB pkb;

			/* Null Test */
			std::vector<int> data;
			Assert::AreEqual(0, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1,4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			Assert::AreEqual(1, pkb.getParent(2));
			Assert::AreEqual(1, pkb.getParent(3));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(4, pkb.getParent(5));
			Assert::AreEqual(2, pkb.getChildren(1)[0]);
			Assert::AreEqual(3, pkb.getChildren(1)[1]);
			Assert::AreEqual(5, pkb.getChildren(4)[0]);
			Assert::AreEqual(true, (pkb.getChildren(2) == data));
		}

		TEST_METHOD(PKBParentStar)
		{
			PKB pkb;

			/* Null Test */
			std::vector<int> data;
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 5, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 6, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 5 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 5 },{ 6 },{ 2 } });

			vector<int> parentStar1 = { 1 };
			vector<int> parentStar2 = { 5 };
			vector<int> childStar1 = { 2, 3, 4 };
			vector<int> childStar2 = { 6 };
			Assert::AreEqual(true, (pkb.getParentStar(1) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(3) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(4) == data));
			Assert::AreEqual(true, (pkb.getParentStar(3) == parentStar1));
			Assert::AreEqual(true, (pkb.getParentStar(6) == parentStar2));
			Assert::AreEqual(true, (pkb.getParentStar(2) == parentStar1));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == childStar1));
			Assert::AreEqual(true, (pkb.getChildrenStar(5) == childStar2));
			Assert::AreEqual(true, (pkb.getChildrenStar(2) == data));
		}

		TEST_METHOD(PKBCheckParent)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(2, 1, { { 0 },{ 1,4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			Assert::AreEqual(true, pkb.checkParent(1, 2));
			Assert::AreEqual(true, pkb.checkParent(1, 3));
			Assert::AreEqual(true, pkb.checkParent(4, 5));
			Assert::AreEqual(false, pkb.checkParent(2, 1));
			Assert::AreEqual(false, pkb.checkParent(3, 1));
			Assert::AreEqual(false, pkb.checkParent(3, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 3));
		}

		TEST_METHOD(PKBCheckParentStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkParentStar(1, 2));
			Assert::AreEqual(false, pkb.checkParentStar(1, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParentStar(1, 2));
			Assert::AreEqual(false, pkb.checkParentStar(1, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			Assert::AreEqual(true, pkb.checkParentStar(1, 2));
			Assert::AreEqual(true, pkb.checkParentStar(1, 3));
			Assert::AreEqual(true, pkb.checkParentStar(1, 4));
			Assert::AreEqual(true, pkb.checkParentStar(1, 5));
			Assert::AreEqual(true, pkb.checkParentStar(4, 5));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));
			Assert::AreEqual(false, pkb.checkParentStar(4, 1));
			Assert::AreEqual(false, pkb.checkParentStar(4, 2));
			Assert::AreEqual(false, pkb.checkParentStar(4, 3));
		}

		TEST_METHOD(PKBGetAllParent)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data = { {1, 2}, {1, 3}, {1, 4}, {4, 5} };
			Assert::AreEqual(true, (pkb.getAllParent() == data));
		}

		TEST_METHOD(PKBGetAllParentStar)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data.insert({ 1, { 2, 3, 4, 5 } });
			data.insert({ 4, { 5 } });
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));
		}

		TEST_METHOD(PKBUsesStatement)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> null_data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{1},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{  },{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{ } });
			pkb.insertToTable(4, 2, { { },{} });

			data = { 1 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(2, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(2) == null_data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(2) == null_data));
		}

		TEST_METHOD(PKBUsesProcedure)
		{
			PKB pkb;

			std::vector<int> data;
			std::vector<int> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));

			pkb.insertToTable(3, 1, { {}, {1},{ } });
			pkb.insertToTable(3, 2, { {}, {1},{ } });
			pkb.insertToTable(4, 1, { {},{1,2} });

			data = { 1 };
			data2 = { 1, 2 };

			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data2));
		}

		TEST_METHOD(PKBUsesContainer)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> data2;
			std::vector<int> data3;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2,3,4,5},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{2},{ 2 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{ 6, 7 },{  },{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ 7 },{  },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(4, 2, { { 1, 2 },{} });
			pkb.insertToTable(4, 3, { { 1, 3 },{} });
			pkb.insertToTable(4, 4, { { 1, 4 },{} });
			pkb.insertToTable(4, 5, { { 1, 5 },{} });
			pkb.insertToTable(4, 6, { { 6 },{} });
			pkb.insertToTable(4, 7, { { 6, 7 },{} });

			data = { 2,3,4,5 };
			data2 = { 1,3 };
			data3 = { 6, 7 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(2, 2));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(6, 6));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(6, 7));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(7, 6));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(7, 7));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(7) == data3));
		}

		TEST_METHOD(PKBGetAllUsesStatement)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> stmts;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 2,3,4,5 },{ 2,3,4,5 },{ 1 } });
			pkb.insertToTable(4, 2, { { 1 },{} });
			pkb.insertToTable(4, 3, { { 2 },{} });
			pkb.insertToTable(4, 4, { { 3 },{} });
			pkb.insertToTable(4, 5, { { 4 },{} });
			
			stmts.insert({ 1,{ 2 } });
			stmts.insert({ 2,{ 3 } });
			stmts.insert({ 3,{ 4 } });
			stmts.insert({ 4,{ 5 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{} });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 1 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 2 } });
			vars.insert({ pkb.insertToNameTable(9, "d") ,{ 3 } });
			vars.insert({ pkb.insertToNameTable(9, "e") ,{ 4 } });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesContainer)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> stmts;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2, 3, 4, 5},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 5, { { 3 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{ 6, 7 },{},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ 7 }, {}, { 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(4, 2, { { 1, 2 },{} });
			pkb.insertToTable(4, 3, { { 1, 3 },{} });
			pkb.insertToTable(4, 4, { { 1, 4 },{} });
			pkb.insertToTable(4, 5, { { 1, 5 },{} });
			pkb.insertToTable(4, 6, { { 6 },{} });
			pkb.insertToTable(4, 7, { { 6, 7 },{} });
			
			stmts.insert({ 1, { 2, 3, 4, 5} });
			stmts.insert({ 2, { 2 } });
			stmts.insert({ 3, { 3 } });
			stmts.insert({ 4, { 4 } });
			stmts.insert({ 5, { 5 } });
			stmts.insert({ 6,{ 6, 7 } });
			stmts.insert({ 7,{ 7 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ } });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 1, 2 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 1,3  } });
			vars.insert({ pkb.insertToNameTable(9, "d") ,{ 1,4  } });
			vars.insert({ pkb.insertToNameTable(9, "e") ,{ 1,5 } });
			vars.insert({ pkb.insertToNameTable(9, "f") ,{ 6 } });
			vars.insert({ pkb.insertToNameTable(9, "g") ,{ 6,7 } });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesProcedure)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> procs;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{ 1 },{},{ 1 } });
			pkb.insertToTable(3, 1, { { 1},{1}, {} });
			pkb.insertToTable(3, 2, { { 2},{ 1 },{} });
			pkb.insertToTable(4, 1, { {}, {1, 2} });

			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });
			procs.insert({ pkb.insertToNameTable(8, "abd"),{ 1 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1, 2 } });

			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));
		}

		TEST_METHOD(PKBModifiesStatement)
		{
			PKB pkb;

			std::vector<int> data;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{1},{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });

			data = { 1 };

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBModifiesContainer)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> data2;
			std::vector<int> data3;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{},{2,3,4,5},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{  },{7},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{  },{ 7 },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(5, 2, { { 1, 2 },{} });
			pkb.insertToTable(5, 3, { { 1, 3 },{} });
			pkb.insertToTable(5, 4, { { 1, 4 },{} });
			pkb.insertToTable(5, 5, { { 1, 5 },{} });
			pkb.insertToTable(5, 6, { {  },{} });
			pkb.insertToTable(5, 7, { { 6, 7 },{} });

			data = { 2,3,4,5 };
			data2 = { 1,3 };
			data3 = { 6, 7 };

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(2, 2));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(6, 7));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(7, 6));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(7, 7));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(7) == data3));
		}

		TEST_METHOD(PKBModifiesProcedure)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));

			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1, 2 } });

			data = { 1 };
			data2 = { 1, 2 };

			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data2));
		}

		TEST_METHOD(PKBGetAllModifiesStatement)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> stmts;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
			
			pkb.insertToTable(1, 1, { { 1 },{  },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{ 2 },{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });
			pkb.insertToTable(5, 2, { { 1 },{} });

			stmts.insert({ 1,{ 1 } });
			stmts.insert({ 2,{ 2 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });
			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 2 } });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesContainer)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> stmts;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));

			pkb = PKB();

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2,3,4,5},{2,3,4,5},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 5, { { 3 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{  },{7},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ },{7},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(5, 2, { { 1, 2 },{} });
			pkb.insertToTable(5, 3, { { 1, 3 },{} });
			pkb.insertToTable(5, 4, { { 1, 4 },{} });
			pkb.insertToTable(5, 5, { { 1, 5 },{} });
			pkb.insertToTable(5, 6, { {  },{} });
			pkb.insertToTable(5, 7, { { 6, 7 },{} });

			stmts.insert({ 1,{ 2, 3, 4, 5 } });
			stmts.insert({ 2,{ 2 } });
			stmts.insert({ 3,{ 3 } });
			stmts.insert({ 4,{ 4 } });
			stmts.insert({ 5,{ 5 } });
			stmts.insert({ 6,{ 7 } });
			stmts.insert({ 7,{ 7 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{} });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 1, 2 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 1,3 } });
			vars.insert({ pkb.insertToNameTable(9, "d") ,{ 1,4 } });
			vars.insert({ pkb.insertToNameTable(9, "e") ,{ 1,5 } });
			vars.insert({ pkb.insertToNameTable(9, "f") ,{  } });
			vars.insert({ pkb.insertToNameTable(9, "g") ,{ 6,7 } });

			unordered_map<int, std::vector<int>> table = pkb.getAllStatementModifiesVariables();
			
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesProcedure)
		{
			PKB pkb;

			unordered_map<int, std::vector<int>> procs;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{ 1 },{ 1 } });
			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1,2 } });


			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });
			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1, 2 } });

			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));
		}
		
		TEST_METHOD(PKBConstant)
		{
			PKB pkb;
			
			std::vector<int> initial_data;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == initial_data));

			pkb.insertToTable(7, 1, { { 1 } });
			std::vector<int> data = {1};
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(2) == initial_data));

		}


	};

}