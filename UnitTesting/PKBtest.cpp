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
				Assert::AreEqual(true, (pkb.getFromTable(i, 1) == data));
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
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 3 } });
			data = { 1, 3 };

			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));
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

			pkb.insertToTable(2, 1, { {0}, {1, 2}, {}, {}, {1} });

			Assert::AreEqual(1, pkb.getFollowsBefore(2));
			Assert::AreEqual(2, pkb.getFollowsAfter(1));
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

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{},{},{ 1 } });

			std::vector<int> dataBeforeStar = {1, 2};
			std::vector<int> dataAfterStar = {2, 3};
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

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 2));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkFollows(1, 2));
		}

		TEST_METHOD(PKBCheckFollowsStar)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkFollowsStar(1, 3));
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

			/* Invalid Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollows().size()));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{},{},{ 1 } });

			Assert::AreEqual(3, static_cast<int>(pkb.getAllFollows().size()));
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
			int size = 0;
			for (auto it = follows.begin(); it != follows.end(); ++it) {
				size += it->second.size();
			}
			Assert::AreEqual(6, size);
		}

		TEST_METHOD(PKBParent)
		{
			PKB pkb;

			/* Null Test */
			std::vector<int> data;
			Assert::AreEqual(0, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(1, 1, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(2, 1, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 2, { { 0 },{ 1 },{},{},{ 1 } });

			Assert::AreEqual(1, pkb.getParent(2));
			Assert::AreEqual(0, pkb.getParent(1));
			Assert::AreEqual(2, pkb.getChildren(1)[0]);
		}

		TEST_METHOD(PKBParentStar)
		{
			PKB pkb;

			/* Null Test */
			std::vector<int> data;
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

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

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(1, 1, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(2, 1, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 2, { { 0 },{ 1 },{},{},{ 1 } });

			Assert::AreEqual(true, pkb.checkParent(1,2));
			Assert::AreEqual(false, pkb.checkParent(2,1));
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

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParentStar(1, 2));
			Assert::AreEqual(false, pkb.checkParentStar(1, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));

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
			std::vector<std::vector<int>> data;
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			data = { {1, 2}, {2, 3} };
			Assert::AreEqual(true, (pkb.getAllParent() == data));
		}

		TEST_METHOD(PKBGetAllParentStar)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{},{},{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{},{},{ 3 } });
			pkb.insertToTable(2, 3, { { 2 },{ 3 },{},{},{ 1 } });

			data.insert({ 1, { 2, 3 } });
			data.insert({ 2, { 3 } });
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));
		}

		TEST_METHOD(PKBUsesStatement)
		{
			PKB pkb;
			std::vector<int> data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{1},{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{ } });

			data = { 1 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBUsesContainer)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{2, 3},{}, { 3 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			pkb.insertToTable(4, 2, { { 2 },{} });
			pkb.insertToTable(4, 3, { { 3, 1 },{} });

			data = { 1, 2, 3 };
			data2 = { 3, 1 };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));
		}

		TEST_METHOD(PKBUsesProcedure)
		{
			PKB pkb;
			std::vector<int> data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));

			pkb.insertToTable(3, 1, { { },{ 1 },{} });
			pkb.insertToTable(4, 1, { { }, { 1} });

			data = { 1 };

			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBGetAllUsesStatement)
		{
			PKB pkb;
			unordered_map<int, std::vector<int>> stmts;
			unordered_map<int, std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			
			stmts.insert({ 1, {1} });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

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

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{ 2, 3 },{},{ 3 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			pkb.insertToTable(4, 2, { { 2, 1 },{} });
			pkb.insertToTable(4, 3, { { 3, 1 },{} });

			stmts.insert({ 1,{ 1, 2, 3 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 2, 1 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 3, 1 } });

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
			pkb.insertToTable(3, 1, { {  },{1}, {} });
			pkb.insertToTable(4, 1, { {}, {1} });

			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

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
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBModifiesContainer)
		{
			PKB pkb;
			std::vector<int> data;
			std::vector<int> data2;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1 },{ },{1},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{  },{ 2, 3 },{ 3 } });
			pkb.insertToTable(5, 1, { { 1 },{} });
			pkb.insertToTable(5, 2, { { 2 },{} });
			pkb.insertToTable(5, 3, { { 3, 1 },{} });

			data = { 1, 2, 3 };
			data2 = { 3, 1 };

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));
		}

		TEST_METHOD(PKBModifiesProcedure)
		{
			PKB pkb;
			std::vector<int> data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));

			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(5, 1, { {},{ 1 } });

			data = { 1 };

			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));
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
			pkb.insertToTable(5, 1, { { 1 },{} });

			stmts.insert({ 1,{ 1 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

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

			pkb.insertToTable(1, 1, { { 1 },{  },{1},{ 3 } });
			pkb.insertToTable(2, 1, { { 1 },{ 2, 3 },{  },{ 2, 3 },{ 3 } });
			pkb.insertToTable(5, 1, { { 1 },{} });
			pkb.insertToTable(5, 2, { { 2, 1 },{} });
			pkb.insertToTable(5, 3, { { 3, 1 },{} });

			stmts.insert({ 1,{ 1, 2, 3 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });
			vars.insert({ pkb.insertToNameTable(9, "b") ,{ 2, 1 } });
			vars.insert({ pkb.insertToNameTable(9, "c") ,{ 3, 1 } });

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
			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(5, 1, { {},{ 1 } });


			procs.insert({ pkb.insertToNameTable(8, "abc"),{ 1 } });

			vars.insert({ pkb.insertToNameTable(9, "a") ,{ 1 } });

			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));
		}

		TEST_METHOD(PKBPatternNoWildcards)
		{
			PKB pkb;
			std::vector<int> data;
			PatternObject p = PatternObject(1, 0, 1, 0);

			/* Null Tests*/
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(false, pkb.checkStatementWithPattern(1, p));

			pkb.insertToTable(1, 1, { { 1 },{1},{ 1 },{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{} });
			pkb.insertToTable(5, 1, { { 1 },{ } });

			data.push_back(1);


			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(true, pkb.checkStatementWithPattern(1, p));
		}

		TEST_METHOD(PKBPatternRHSWildcard)
		{
			PKB pkb;
			std::vector<int> data;
			PatternObject p = PatternObject(1, 0, 0, 1);

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(false, pkb.checkStatementWithPattern(1, p));
			Assert::AreEqual(false, pkb.checkStatementWithPattern(2, p));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{ 1 },{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{ 1 },{ 1 },{ 1 } });
			pkb.insertToTable(4, 1, { { 1, 2 },{} });
			pkb.insertToTable(5, 1, { { 1, 2 },{} });


			data.push_back(1);
			data.push_back(2);

			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(true, pkb.checkStatementWithPattern(1, p));
			Assert::AreEqual(true, pkb.checkStatementWithPattern(2, p));
		}

		TEST_METHOD(PKBPatternLHSWildcard)
		{
			PKB pkb;
			std::vector<int> data;
			PatternObject p = PatternObject(0, 1, 1, 0);

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(false, pkb.checkStatementWithPattern(1, p));
			Assert::AreEqual(false, pkb.checkStatementWithPattern(2, p));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{ 1 },{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{ 1 },{ 1 },{ 1 } });
			pkb.insertToTable(4, 1, { { 1, 2 },{} });
			pkb.insertToTable(5, 1, { { 1, 2 },{} });

			data.push_back(1);
			data.push_back(2);


			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == data));
			Assert::AreEqual(true, pkb.checkStatementWithPattern(1, p));
			Assert::AreEqual(true, pkb.checkStatementWithPattern(2, p));
		}
	};

}