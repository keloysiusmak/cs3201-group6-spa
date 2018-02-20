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

		TEST_METHOD(PKBGetAllVariables)
		{
			PKB pkb;
			std::vector<int> data;

			data.push_back(pkb.insertToNameTable(9, "a"));
			data.push_back(pkb.insertToNameTable(9, "b"));

			Assert::AreEqual(true, (pkb.getAllVariables() == data));
		}

		TEST_METHOD(PKBGetAllProcedures)
		{
			PKB pkb;
			std::vector<int> data;

			data.push_back(pkb.insertToNameTable(8, "a"));
			data.push_back(pkb.insertToNameTable(8, "b"));

			Assert::AreEqual(true, (pkb.getAllProcedures() == data));
		}

		TEST_METHOD(PKBGetAllStatements)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			std::vector<int> data = { 1, 2, 3 };

			Assert::AreEqual(true, (pkb.getAllStatements() == data));
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

		TEST_METHOD(PKBParent)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 2, { { 0 },{ 1 },{},{},{ 1 } });

			Assert::AreEqual(1, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(2, pkb.getChildren(1)[0]);
		}

		TEST_METHOD(PKBParentStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			vector<int> parentStar1 = { 2, 1 };
			vector<int> parentStar2 = { 1 };
			vector<int> childStar1 = { 2, 3 };
			Assert::AreEqual(true, (pkb.getParentStar(3) == parentStar1));
			Assert::AreEqual(true, (pkb.getParentStar(2) == parentStar2));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == childStar1));
		}

		TEST_METHOD(PKBCheckParent)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 2, { { 0 },{ 1 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkParent(1,2));
			Assert::AreEqual(false, pkb.checkParent(2,1));
		}

		TEST_METHOD(PKBCheckParentStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkParentStar(1, 2));
			Assert::AreEqual(true, pkb.checkParentStar(1, 3));
			Assert::AreEqual(true, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));
		}

		TEST_METHOD(PKBGetAllParent)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			std::vector<std::vector<int>> data = { {1, 2}, {2, 3} };
			Assert::AreEqual(true, (pkb.getAllParent() == data));
		}

		TEST_METHOD(PKBGetAllParentStar)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			unordered_map<int, std::vector<int>> data;
			data.insert({ 1, { 2, 3 } });
			data.insert({ 2, { 3 } });
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));
		}

		TEST_METHOD(PKBUsesStatement)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{1},{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{ } });

			std::vector<int> data = { 1 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBUsesContainer)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{2, 3},{}, { 3 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			pkb.insertToTable(4, 2, { { 2 },{} });
			pkb.insertToTable(4, 3, { { 3, 1 },{} });

			std::vector<int> data = { 1, 2, 3 };
			std::vector<int> data2 = { 3, 1 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));
		}

		TEST_METHOD(PKBUsesProcedure)
		{
			PKB pkb;

			pkb.insertToTable(3, 1, { { },{ 1 },{} });
			pkb.insertToTable(4, 1, { { }, { 1} });

			std::vector<int> data = { 1 };

			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBGetAllUsesStatement)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			

			unordered_map<int, std::vector<int>> stmts;
			stmts.insert({ 1, {1} });

			unordered_map<int, std::vector<int>> vars;
			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesContainer)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{ 2, 3 },{},{ 3 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			pkb.insertToTable(4, 2, { { 2, 1 },{} });
			pkb.insertToTable(4, 3, { { 3, 1 },{} });

			unordered_map<int, std::vector<int>> stmts;
			stmts.insert({ 1,{ 1, 2, 3 } });

			unordered_map<int, std::vector<int>> vars;
			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 2, 1 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 3, 1 } });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesProcedure)
		{
			PKB pkb;

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(3, 1, { {  },{1}, {} });
			pkb.insertToTable(4, 1, { {}, {1} });


			unordered_map<int, std::vector<int>> procs;
			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });

			unordered_map<int, std::vector<int>> vars;
			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));
		}
	};

}