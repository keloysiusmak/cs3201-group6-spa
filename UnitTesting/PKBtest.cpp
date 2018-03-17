#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB.h"
#include "../SPA/Pattern.h"
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{ 
	TEST_CLASS(PKBTest)
	{
	public:
		TEST_METHOD(PKBGetFromTable)
		{
			PKB pkb;

			std::vector<std::vector<int>> null;
			std::vector<std::vector<int>> data = { { 1 },{},{},{} };
			pkb.insertToTable(STATEMENT_TABLE, 1, data);

			Assert::AreEqual(true, (data == pkb.getFromTable(STATEMENT_TABLE, 1)));
			Assert::AreEqual(true, (null == pkb.getFromTable(STATEMENT_TABLE, 2)));
			Assert::AreEqual(true, (null == pkb.getFromTable(99999, 2)));
		}
		TEST_METHOD(PKBInsertToTable)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));
			
			int tableValuesCount;

			for (int i = 1; i < PATTERN_TABLE; i++) {
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
					tableValuesCount = 1;
					break;
				case 7:
					tableValuesCount = 2;
					break;
				case 8:
					tableValuesCount = 1;
					break;
				case 9:
					tableValuesCount = 1;
					break;
				case 10:
					tableValuesCount = 1;
					break;
				case 11:
					tableValuesCount = 1;
					break;
				case 12:
					tableValuesCount = 1;
					break;
				case 13:
					tableValuesCount = 1;
					break;
				case 14:
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

		TEST_METHOD(PKBInsertToTableInvalid)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			int tableValuesCount;

			for (int i = 1; i < PROC_TABLE; i++) {
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
					tableValuesCount = 1;
					break;
				case 7:
					tableValuesCount = 2;
					break;
				case 8:
					tableValuesCount = 1;
					break;
				case 9:
					tableValuesCount = 2;
					break;
				case 10:
					tableValuesCount = 1;
					break;
				case 11:
					tableValuesCount = 1;
					break;
				case 12:
					tableValuesCount = 1;
					break;
				case 13:
					tableValuesCount = 1;
					break;
				case 14:
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

		TEST_METHOD(PKBInsertToTableMultiple)
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

		TEST_METHOD(PKBInsertToTableInsertMultipleSameValue)
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

		TEST_METHOD(PKBInsertToTableMultipleInvalid)
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

		TEST_METHOD(PKBInsertToNameTable)
		{
			PKB pkb;

			string data = "x";
			std::vector<string> null;

			for (int i = PATTERN_TABLE; i < PATTERN_TABLE + 3; i++) {
				/* Null Test */
				null.clear();
				Assert::AreEqual(true, (null == pkb.getFromNameTable(i, 1)));
				if (i > PATTERN_TABLE) {
					null = { "x" };
					Assert::AreEqual(true, (null == pkb.getFromNameTable(i, pkb.insertToNameTable(i, { data }))));
				}
				else {
					null = {"x", "x"};
					Assert::AreEqual(true, (null == pkb.getFromNameTable(i, pkb.insertToNameTable(i, { data, data }))));
				}
			}
		}

		TEST_METHOD(PKBInsertToNameTableInvalid)
		{
			PKB pkb;

			std::vector<string> null;
			string data = "x";

			for (int i = PROC_TABLE; i < PROC_TABLE + 2; i++) {
				/* Null Test */
				Assert::AreEqual(true, { null == pkb.getFromNameTable(i, 1) });

				Assert::AreEqual(1, pkb.insertToNameTable(i, { data }));
				Assert::AreEqual(1, pkb.insertToNameTable(i, { data }));
			}
		}

		TEST_METHOD(PKBGetProcedureName)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;
			Assert::AreEqual(true, (null == pkb.getFromNameTable(PROC_TABLE, 1)));

			string data = "proc_x" ;

			int return_id;

			return_id = pkb.insertToNameTable(PROC_TABLE, { data });
			Assert::AreEqual(true, ("proc_x" == pkb.getProcedureName(return_id)));
		}

		TEST_METHOD(PKBGetVariableName)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;

			null = {};
			Assert::AreEqual(true, (null == pkb.getFromNameTable(VAR_TABLE, 1)));

			string data = "x";

			int return_id;

			return_id = pkb.insertToNameTable(VAR_TABLE, { data });
			Assert::AreEqual(true, ("x" == pkb.getVariableName(return_id)));
		}

		TEST_METHOD(PKBGetProcedureId)
		{
			PKB pkb;
			
			string data = "proc_x";

			pkb.insertToNameTable(PROC_TABLE, { data });
			Assert::AreEqual(1, pkb.getProcedureId("proc_x"));
			Assert::AreEqual(0, pkb.getProcedureId("PROC_X"));
		}

		TEST_METHOD(PKBGetVariableId)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;
			null = {};
			Assert::AreEqual(true, (null == pkb.getFromNameTable(VAR_TABLE, 1)));

			string data = "x";

			pkb.insertToNameTable(VAR_TABLE, { data });
			Assert::AreEqual(1, pkb.getVariableId("x"));
			Assert::AreEqual(0, pkb.getVariableId("X"));
		}

		TEST_METHOD(PKBGetAllVariables)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllVariables() == data));

			data.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) });
			data.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) });

			Assert::AreEqual(true, (pkb.getAllVariables() == data));
		}

		TEST_METHOD(PKBGetAllProcedures)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllProcedures() == data));

			data.push_back({ pkb.insertToNameTable(PROC_TABLE, {"a"}) });
			data.push_back({ pkb.insertToNameTable(PROC_TABLE, {"b"}) });

			Assert::AreEqual(true, (pkb.getAllProcedures() == data));
		}

		TEST_METHOD(PKBGetAllStatements)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatements() == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			data = { { 1}, {2}, {3} };


			Assert::AreEqual(true, (pkb.getAllStatements() == data));
		}

		TEST_METHOD(PKBGetAllStatementsWithType)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 2 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 3 } });

			data = { { 1}, {3 } };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			data = { { 2 } };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(2) == data));
		}

		TEST_METHOD(PKBFollows)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Tests */
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));

			pkb.insertToTable(1, 1, { {1}, {}, {}, {1} });
			pkb.insertToTable(1, 2, { {1}, {}, {}, {1} });

			/* Invalid Tests */
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));

			pkb.insertToTable(2, 1, { {0}, {1, 2}, {1} });

			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			data = { { 2 } };
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));
			data.clear();
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(0)));
		}

		TEST_METHOD(PKBFollowsStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{ 1 } });

			std::vector<std::vector<int>> dataBeforeStar = { {1}, {2} };
		std::vector<std::vector<int>> dataAfterStar = { {2}, {3} };
			std::vector<std::vector<int>> nullData;
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

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 1 } });
			
			std::vector<std::vector<int>> follows = pkb.getAllFollowsStar();
			std::vector<std::vector<int>> data = { {1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4} };
			for (int i = 0; i < follows[0].size(); i++) {
				Assert::AreEqual(true, (follows[0][i] == data[0][i]));
				i++;
			}

		}

		TEST_METHOD(PKBParent)
		{
			PKB pkb;

			/* Null Test */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1,4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getParent(3)));
			data = { { 0 } };
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			data = { { 4 } };
			Assert::AreEqual(true, (data == pkb.getParent(5)));

			std::vector<std::vector<int>> result;
			result = { {2}, {3} };
			Assert::AreEqual(true, ( result == pkb.getChildren(1)));
			result = { {5} };
			Assert::AreEqual(true, (result == pkb.getChildren(4)));
			data.clear();
			Assert::AreEqual(true, (pkb.getChildren(2) == data));
		}

		TEST_METHOD(PKBParentStar)
		{
			PKB pkb;

			/* Null Test */
			std::vector<std::vector<int>> data;
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

			std::vector<std::vector<int>> parentStar1 = { { 1 } };
			std::vector<std::vector<int>> parentStar2 = { { 5 } };
			std::vector<std::vector<int>> childStar1 = { { 2}, {3}, {4} };
			std::vector<std::vector<int>> childStar2 = { { 6 } };
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
			std::vector<std::vector<int>> data;

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

			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,4 });
			data.push_back({ 1,5 });
			data.push_back({ 4, 5 });

			Assert::AreEqual(true, (pkb.getAllParentStar() == data));
		}

		TEST_METHOD(PKBUsesStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> null_data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{1},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{  },{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{ } });
			pkb.insertToTable(4, 2, { { },{} });

			data = { { 1 } };

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

			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));

			pkb.insertToTable(3, 1, { {}, {1},{ } });
			pkb.insertToTable(3, 2, { {}, {1},{ } });
			pkb.insertToTable(4, 1, { {},{1,2} });

			data = { { 1 } };
			data2 = { { 1}, {2 }};

			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data2));
		}

		TEST_METHOD(PKBUsesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;
			std::vector<std::vector<int>> data3;

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

			data = { { 2},{3},{4},{5} };
			data2 = { { 1},{3 }};
			data3 = { { 6}, {7 }};

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
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

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
			
			stmts.push_back({1,2});
			stmts.push_back({ 2,3 });
			stmts.push_back({ 3,4 });
			stmts.push_back({ 4,5 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 4 });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

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
			
			stmts.push_back({ 1, 2 });
			stmts.push_back({ 1, 3 });
			stmts.push_back({ 1, 4 });
			stmts.push_back({ 1, 5 });
			stmts.push_back({ 2, 2 });
			stmts.push_back({ 3, 3 });
			stmts.push_back({ 4, 4 });
			stmts.push_back({ 5, 5 });
			stmts.push_back({ 6, 6 });
			stmts.push_back({ 6, 7 });
			stmts.push_back({ 7, 7 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) ,1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) ,2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 4 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 5 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"f"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 7 });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesProcedure)
		{
			PKB pkb;
			std::vector<std::vector<int>> procs;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{ 1 },{},{ 1 } });
			pkb.insertToTable(3, 1, { { 1},{1}, {} });
			pkb.insertToTable(3, 2, { { 2},{ 1 },{} });
			pkb.insertToTable(4, 1, { {}, {1, 2} });

			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abc"}), 1 });
			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abd"}), 1 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 2 });

			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));
		}

		TEST_METHOD(PKBModifiesStatement)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{1},{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });

			data = { { 1 } };

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBModifiesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;
			std::vector<std::vector<int>> data3;
			
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

			data = { { 2},{3},{4},{5 }};
			data2 = { { 1 }, {3 } };
			data3 = { { 6}, {7 }};

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
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));

			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1, 2 } });

			data = { { 1 } };
			data2 = { { 1}, {2 }};

			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data2));
		}

		TEST_METHOD(PKBGetAllModifiesStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
			
			pkb.insertToTable(1, 1, { { 1 },{  },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{ 2 },{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });
			pkb.insertToTable(5, 2, { { 1 },{} });

			stmts.push_back({ 1, 1 });
			stmts.push_back({ 2, 2 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

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

			stmts.push_back({ 1,2 });
			stmts.push_back({ 1,3 });
			stmts.push_back({ 1,4 });
			stmts.push_back({ 1,5 });
			stmts.push_back({ 2,2 });
			stmts.push_back({ 3,3 });
			stmts.push_back({ 4,4 });
			stmts.push_back({ 5,5 });
			stmts.push_back({ 6,7 });
			stmts.push_back({ 7,7 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 4 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 5 });
			pkb.insertToNameTable(VAR_TABLE, { "f" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 7 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesProcedure)
		{
			PKB pkb;

			std::vector<std::vector<int>> procs;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{ 1 },{ 1 } });
			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1,2 } });


			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abc"}), 1 });
			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abd"}), 1 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 2 });

			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));
		}

		TEST_METHOD(PKBGetNextBefore)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextBefore(3) == data));
			Assert::AreEqual(true, (pkb.getNextBefore(1) == data));

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 1 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getNextBefore(1) == data));

			/* Valid Tests */
			data = { {1} };
			Assert::AreEqual(true, (pkb.getNextBefore(2) == data));
			Assert::AreEqual(true, (pkb.getNextBefore(3) == data));
		}

		TEST_METHOD(PKBGetNextAfter)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextAfter(2) == data));
			Assert::AreEqual(true, (pkb.getNextAfter(1) == data));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2,3 } });

			/* Valid Tests */
			data = { { 2 }, {3} };
			Assert::AreEqual(true, (pkb.getNextAfter(1) == data));
			data.clear();
			Assert::AreEqual(true, (pkb.getNextAfter(2) == data));
		}

		TEST_METHOD(PKBGetNextBeforeStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getNextBeforeStar(1) == data));

			pkb.insertToTable(NEXT_STAR_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(NEXT_STAR_INVERSE_TABLE, 3, { { 1,2 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getNextBeforeStar(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getNextBeforeStar(2) == data));
			data = { { 1 }, {2} };
			Assert::AreEqual(true, (pkb.getNextBeforeStar(3) == data));
		}

		TEST_METHOD(PKBGetNextAfterStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextAfterStar(2) == data));
			Assert::AreEqual(true, (pkb.getNextAfterStar(1) == data));

			pkb.insertToTable(NEXT_STAR_TABLE, 1, { { 2,3 } });
			pkb.insertToTable(NEXT_STAR_TABLE, 2, { { 3 } });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getNextAfterStar(1) == data));
			data = { { 3 } };
			Assert::AreEqual(true, (pkb.getNextAfterStar(2) == data));
		}

		TEST_METHOD(PKBGetAllNext)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllNext() == data));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2,5 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3,4 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 1 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 1 } });

			/* Valid Tests */
			data = { { 1,2 },{ 1,5 }, {2,3}, {2,4}, {3,1}, {4,1} };
			Assert::AreEqual(true, (pkb.getAllNext() == data));
		}

		TEST_METHOD(PKBGetAllNextStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllNextStar() == data));

			pkb.insertToTable(NEXT_STAR_TABLE, 1, { { 1,2,3 } });
			pkb.insertToTable(NEXT_STAR_TABLE, 2, { { 1,2,3 } });
			pkb.insertToTable(NEXT_STAR_TABLE, 3, { { 1,2,3 } });

			/* Valid Tests */
			data = { {1,1}, { 1,2 },{1,3}, {2,1}, {2,2},{2,3}, {3,1}, {3,2}, {3,3} };
			Assert::AreEqual(true, (pkb.getAllNextStar() == data));
		}

		TEST_METHOD(PKBCheckNext)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkNext(1,2));

			pkb.insertToTable(NEXT_TABLE, 1, { {2} });
			pkb.insertToTable(NEXT_TABLE, 2, { {3} });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkNext(1,2));
			Assert::AreEqual(true, pkb.checkNext(2, 3));
			Assert::AreEqual(false, pkb.checkNext(1, 3));
		}

		TEST_METHOD(PKBCheckNextStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkNextStar(1, 2));

			pkb.insertToTable(NEXT_STAR_TABLE, 1, { { 2,3,4 } });
			pkb.insertToTable(NEXT_STAR_TABLE, 2, { { 2,3,4 } });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkNextStar(1, 2));
			Assert::AreEqual(true, pkb.checkNextStar(1, 3));
			Assert::AreEqual(true, pkb.checkNextStar(1, 4));
			Assert::AreEqual(true, pkb.checkNextStar(2, 2));
			Assert::AreEqual(true, pkb.checkNextStar(2, 3));
			Assert::AreEqual(true, pkb.checkNextStar(2, 4));
			Assert::AreEqual(false, pkb.checkNextStar(1, 1));
			Assert::AreEqual(false, pkb.checkNextStar(3, 1));
		}

		TEST_METHOD(PKBGetCallsBefore)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsBefore(3) == data));
			Assert::AreEqual(true, (pkb.getCallsBefore(1) == data));

			pkb.insertToTable(CALLS_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_INVERSE_TABLE, 3, { { 1 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getCallsBefore(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getCallsBefore(2) == data));
			Assert::AreEqual(true, (pkb.getCallsBefore(3) == data));
		}

		TEST_METHOD(PKBGetCallsAfter)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsAfter(2) == data));
			Assert::AreEqual(true, (pkb.getCallsAfter(1) == data));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2,3 },{} });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfter(1) == data));
			data.clear();
			Assert::AreEqual(true, (pkb.getCallsAfter(2) == data));
		}

		TEST_METHOD(PKBGetCallsBeforeStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(1) == data));

			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 3, { { 1,2 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(2) == data));
			data = { { 1 },{ 2 } };
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(3) == data));
		}

		TEST_METHOD(PKBGetCallsAfterStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsAfterStar(2) == data));
			Assert::AreEqual(true, (pkb.getCallsAfterStar(1) == data));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 3 } });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfterStar(1) == data));
			data = { { 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfterStar(2) == data));
		}

		TEST_METHOD(PKBGetAllCalls)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllCalls() == data));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2,5 },{} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3,4 },{} });
			pkb.insertToTable(CALLS_TABLE, 3, { { 1 },{} });
			pkb.insertToTable(CALLS_TABLE, 4, { { 1 },{} });

			/* Valid Tests */
			data = { { 1,2 },{ 1,5 },{ 2,3 },{ 2,4 },{ 3,1 },{ 4,1 } };
			Assert::AreEqual(true, (pkb.getAllCalls() == data));
		}

		TEST_METHOD(PKBGetAllCallsStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllCallsStar() == data));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 1,2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 1,2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 3, { { 1,2,3 } });

			/* Valid Tests */
			data = { { 1,1 },{ 1,2 },{ 1,3 },{ 2,1 },{ 2,2 },{ 2,3 },{ 3,1 },{ 3,2 },{ 3,3 } };
			Assert::AreEqual(true, (pkb.getAllCallsStar() == data));
		}

		TEST_METHOD(PKBCheckCalls)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkCalls(1, 2));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2 }, {} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3 }, {} });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkCalls(1, 2));
			Assert::AreEqual(true, pkb.checkCalls(2, 3));
			Assert::AreEqual(false, pkb.checkCalls(1, 3));
		}

		TEST_METHOD(PKBCheckCallsStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkCallsStar(1, 2));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2,3,4 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 2,3,4 } });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkCallsStar(1, 2));
			Assert::AreEqual(true, pkb.checkCallsStar(1, 3));
			Assert::AreEqual(true, pkb.checkCallsStar(1, 4));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 2));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 3));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 4));
			Assert::AreEqual(false, pkb.checkCallsStar(1, 1));
			Assert::AreEqual(false, pkb.checkCallsStar(3, 1));
		}
		
		TEST_METHOD(PKBGetStatementsWithPattern)
		{
			PKB pkb;

			Pattern p;
			Param ep;
			Param lp;
			Param rp;

			std::vector<std::vector<int>> null;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == null));

			//a = (b + c) * d + e * f + g; 
			//h = d + e;
			//i = e * f;
			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });
			pkb.insertToNameTable(VAR_TABLE, { "f" });
			pkb.insertToNameTable(VAR_TABLE, { "g" });
			pkb.insertToNameTable(VAR_TABLE, { "h" });
			pkb.insertToNameTable(VAR_TABLE, { "i" });
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1}, {2,3,4,5,6,7}, {1}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 4,5 },{ 8 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1 },{ 5,6 },{ 9 },{ 1 } });
			pkb.insertToNameTable(PATTERN_TABLE, {"a", "b|c|+|d|*|e|f|*|+|g|+|"});
			pkb.insertToNameTable(PATTERN_TABLE, { "h", "d|e|+|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "i", "e|f|*|" });

			//a(_,_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p1(ep, lp, rp);
			std::vector<std::vector<int>> expected = { {1}, {2}, {3} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p1) == expected));

			//a(_,_"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p2(ep, lp, rp);
			expected = { {1},{3} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p2) == expected));

			//a(_,"e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p3(ep, lp, rp);
			expected = { { 3 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p3) == expected));

			//a(v, _)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p4(ep, lp, rp);
			expected = { {1,1}, {2,8}, { 3,9 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p4) == expected));

			//a("a", _)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p5(ep, lp, rp);
			expected = { { 1 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p5) == expected));

			//a(v,_"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p6(ep, lp, rp);
			expected = { {1, 1}, {3, 9} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p6) == expected));

			//a(v,"e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p7(ep, lp, rp);
			expected = { { 3, 9 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p7) == expected));

			//a("a", _"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p8(ep, lp, rp);
			expected = { { 1 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p8) == expected));

			//a("a", "e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "x";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p9(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p9) == expected));

			//a(_, _"e*f+g"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|g|+|";
			rp.attribute = NONE;

			Pattern p10(ep, lp, rp);
			expected = { {1} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p10) == expected));

			//a("y", _"e*f+g"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "y";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|g|+|";
			rp.attribute = NONE;

			Pattern p11(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p11) == expected));

		}
		
		TEST_METHOD(PKBConstant)
		{
			PKB pkb;
			
			std::vector<std::vector<int>> initial_data;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == initial_data));

			pkb.insertToTable(CONST_TABLE, 1, { { 1 } });
			std::vector<std::vector<int>> data = { {1} };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(2) == initial_data));

		}


	};

}