#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/Parser.h";
#include "../SPA/DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBDEParserIntegrationTesting
{
	PKB pkb;
	Parser parser;
	string simpleSource;
	string testString;
	DesignExtractor de;

	TEST_CLASS(PKBDEParserIntegration)
	{
	public:

		TEST_CLASS_INITIALIZE(setup)
		{
			parser = Parser();
			simpleSource = "";
			/* 
				procedure a{ 
				1.	a = b; 
				2.	while c { 
				3.		b = a;
				4.		call b; } 
				5.	if a then { 
				6.		while e { 
				7.			c = 4; }} 
					else { 
				8.		d = 1; } 
				9.	e = 1; }

				procedure b{
				10.	g = b;
				11.	while h {
				12.		while i {
				13.			j = 1; }}}
			
			*/
			testString = "procedure a {a = b; while c { b = a; d = 7; } if a then { while e { c = 4; } } else { d = 1; } e = 1; } procedure b{ f = g; while h { while i { j = 1; }} }";
			pkb = parser.Parse(simpleSource, pkb, true, testString);
			de.extract(pkb);
		}

		TEST_METHOD(PKBDEParserGetFollowsBeforeInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(1)));
		}

		TEST_METHOD(PKBDEParserGetFollowsBefore)
		{
			std::vector<std::vector<int>> data = { {1} };
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
		}

		TEST_METHOD(PKDEBParserGetFollowsAfter)
		{
			std::vector<std::vector<int>> data = { {2} };
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));
		}

		TEST_METHOD(PKBDEParserGetFollowsAfterInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(13)));
		}


		TEST_METHOD(PKBDEParserGetFollowsBeforeStarInvalid)
		{
			std::vector<std::vector<int>> data;;
			Assert::AreEqual(true, (data == pkb.getFollowsBeforeStar(1)));
		}

		TEST_METHOD(PKBDEParserGetFollowsBeforeStar)
		{
			std::vector<std::vector<int>> data = { {1}, {2}, {5} };
			Assert::AreEqual(true, (data == pkb.getFollowsBeforeStar(9)));
		}

		TEST_METHOD(PKBDEParserGetFollowsAfterStar)
		{
			std::vector<std::vector<int>> data = { {5}, {9} };
			Assert::AreEqual(true, (data == pkb.getFollowsAfterStar(2)));
		}

		TEST_METHOD(PKBDEParserGetFollowsAfterStarInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getFollowsAfterStar(9)));
		}

		TEST_METHOD(PKBDEParserCheckFollows)
		{
			Assert::AreEqual(true, pkb.checkFollows(1, 2));
		}

		TEST_METHOD(PKBDEParserCheckFollowsInvalid)
		{
			Assert::AreEqual(false, pkb.checkFollows(2, 3));
		}

		TEST_METHOD(PKBDEParserCheckFollowsStar)
		{
			Assert::AreEqual(true, pkb.checkFollowsStar(1, 9));
		}

		TEST_METHOD(PKBDEParserCheckFollowsStarInvalid)
		{
			Assert::AreEqual(false, pkb.checkFollowsStar(9, 2));
		}

		TEST_METHOD(PKBDEParserGetAllFollows)
		{
			std::vector <std::vector<int>> output = pkb.getAllFollows();
			std::vector <std::vector<int>> data = { {3,4},{ 1,2 },{ 2,5 },{ 5,9 } ,{10,11} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBDEParserGetAllFollowsStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 3,4 });
			data.push_back({ 1,2 });
			data.push_back({ 1,5 });
			data.push_back({ 1,9 });
			data.push_back({ 2,5 });
			data.push_back({ 2,9 });
			data.push_back({ 5,9 });
			data.push_back({ 10,11 });
			std::vector<std::vector<int>> output = pkb.getAllFollowsStar();

			Assert::AreEqual(true, (data == output));
		}

		TEST_METHOD(PKBDEParserGetParentInvalid)
		{
			std::vector<std::vector<int>> data = { {0} };
			Assert::AreEqual(true, (data == pkb.getParent(1)));
		}

		TEST_METHOD(PKBDEParserGetParent)
		{
			std::vector<std::vector<int>> data = { {2} };
			Assert::AreEqual(true, (data == pkb.getParent(3)));
		}

		TEST_METHOD(PKBDEParserGetChildren)
		{
			std::vector<std::vector<int>> data = { {3}, {4} };
			Assert::AreEqual(true, (data == pkb.getChildren(2)));
		}

		TEST_METHOD(PKBDEParserGetChildrenInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getChildren(1)));
		}


		TEST_METHOD(PKBDEParserGetParentStarInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getParentStar(1)));
		}

		TEST_METHOD(PKBDEParserGetParentStar)
		{
			std::vector<std::vector<int>> data = { {6}, {5} };
			Assert::AreEqual(true, (data == pkb.getParentStar(7)));
		}

		TEST_METHOD(PKBDEParserGetChildrenStar)
		{
			std::vector<std::vector<int>> data = { {6}, {8}, {7} };
			Assert::AreEqual(true, (data == pkb.getChildrenStar(5)));
		}

		TEST_METHOD(PKBDEParserGetChildrenStarInvalid)
		{
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getChildrenStar(1)));
		}

		TEST_METHOD(PKBDEParserCheckParent)
		{
			Assert::AreEqual(true, pkb.checkParent(2, 3));
		}

		TEST_METHOD(PKBDEParserCheckParentInvalid)
		{
			Assert::AreEqual(false, pkb.checkParent(1, 2));
		}

		TEST_METHOD(PKBDEParserCheckParentStar)
		{
			Assert::AreEqual(true, pkb.checkParentStar(5, 7));
		}

		TEST_METHOD(PKBDEParserCheckParentStarInvalid)
		{
			Assert::AreEqual(false, pkb.checkParentStar(7, 5));
		}

		TEST_METHOD(PKBDEParserGetAllParent)
		{
			std::vector <std::vector<int>> output = pkb.getAllParent();
			std::vector <std::vector<int>> data = { { 2, 3 },{ 2, 4 },{ 5, 6 },{ 6, 7 } ,{ 5, 8 } , {11,12}, {12,13} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBDEParserGetAllParentStar)
		{
			std::vector <std::vector<int>> data;
			data.push_back({ 2,3 });
			data.push_back({ 2,4 });
			data.push_back({ 6,7 });
			data.push_back({ 5,6 });
			data.push_back({ 5,8 });
			data.push_back({ 5,7 });
			data.push_back({ 12,13 });
			data.push_back({ 11,12 });
			data.push_back({ 11,13 });
			std::vector <std::vector<int>> output = pkb.getAllParentStar();

			Assert::AreEqual(true, (data == output));
		}

		TEST_METHOD(PKBDEParserGetUsesVariablesFromStatement)
		{
			std::vector<std::vector<int>> data = { {2} };
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
		}

		TEST_METHOD(PKBDEParserGetStatementsFromUsesVariables)
		{
			std::vector<std::vector<int>> data = { {2},{3},{5},{11} };
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBDEParserGetUsesVariablesFromProcedure)
		{
			std::vector<std::vector<int>> data = { {1},{2},{3},{5} };
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBDEParserGetProceduresFromUsesVariable)
		{
			std::vector<std::vector<int>> data = { {1},{2} };
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBDEParserGetAllStatementUsesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			data.push_back({ 3,1 });
			data.push_back({ 2,1 });
			data.push_back({ 2,3 });
			data.push_back({ 6,5 });
			data.push_back({ 5,1 });
			data.push_back({ 5,5 });
			data.push_back({ 10,2 });
			data.push_back({ 12,2 });
			data.push_back({ 11,1 });
			data.push_back({ 11,2 });
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableUsesStatements)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,5 });
			data.push_back({ 1,11 });
			data.push_back({ 2,1 });
			data.push_back({ 2,10 });
			data.push_back({ 2,11 });
			data.push_back({ 2,12 });
			data.push_back({ 3,2 });
			data.push_back({ 5,5 });
			data.push_back({ 5,6 });

			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == data));
		}

		TEST_METHOD(PKBDEParserGetAllProcedureUsesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,5 });
			data.push_back({ 2,1 });
			data.push_back({ 2,2 });
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableUsesProcedures)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,2 });
			data.push_back({ 2,1 });
			data.push_back({ 2,2 });
			data.push_back({ 3,1 });
			data.push_back({ 5,1 });
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == data));
		}

		TEST_METHOD(PKBDEParserCheckStatementUsesVariable)
		{
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 3));
		}

		TEST_METHOD(PKBDEParserCheckProcedureUsesVariable)
		{
			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(2, 3));
		}

		TEST_METHOD(PKBDEParserGetModifiesVariablesFromStatement)
		{
			std::vector<std::vector<int>> data = { {1} };
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
		}

		TEST_METHOD(PKBDEParserGetStatementsFromModifiesVariables)
		{
			std::vector<std::vector<int>> data = { {1},{10},{11},{12},{13} };
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBDEParserGetModifiesVariablesFromProcedure)
		{
			std::vector<std::vector<int>> data = { {1},{2},{3},{4},{5} };
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBDEParserGetProceduresFromModifiesVariable)
		{
			std::vector<std::vector<int>> data = { {1},{2} };
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBDEParserGetAllStatementModifiesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 3,2 });
			data.push_back({ 2,2 });
			data.push_back({ 2,4 });
			data.push_back({ 4,4 });
			data.push_back({ 6,3 });
			data.push_back({ 7,3 });
			data.push_back({ 5,3 });
			data.push_back({ 5,4 });
			data.push_back({ 8,4 });
			data.push_back({ 9,5 });
			data.push_back({ 10,1 });
			data.push_back({ 12,1 });
			data.push_back({ 11,1 });
			data.push_back({ 13,1 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableModifiesStatements)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,10 });
			data.push_back({ 1,11 });
			data.push_back({ 1,12 });
			data.push_back({ 1,13 });
			data.push_back({ 2,2 });
			data.push_back({ 2,3 });
			data.push_back({ 3,5 });
			data.push_back({ 3,6 });
			data.push_back({ 3,7 });
			data.push_back({ 4,2 });
			data.push_back({ 4,4 });
			data.push_back({ 4,5 });
			data.push_back({ 4,8 });
			data.push_back({ 5,9 });

			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == data));
		}

		TEST_METHOD(PKBDEParserGetAllProcedureModifiesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,4 });
			data.push_back({ 1,5 });
			data.push_back({ 2,1 });
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableModifiesProcedures)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,2 });
			data.push_back({ 2,1 });
			data.push_back({ 3,1 });
			data.push_back({ 4,1 });
			data.push_back({ 5,1 });
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == data));
		}

		TEST_METHOD(PKBDEParserCheckStatementModifiesVariable)
		{
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 2));
		}

		TEST_METHOD(PKBDEParserCheckProcedureModifiesVariable)
		{
			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(2, 2));
		}

		TEST_METHOD(PKBDEParserGetStatementsWithConstant)
		{
			std::vector<std::vector<int>> data = { {8},{9},{13} };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
		}
	};
}