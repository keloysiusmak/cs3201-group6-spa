#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBParserIntegrationTesting
{
	PKB pkb;
	Parser parser;
	string simpleSource;
	string testString;

	TEST_CLASS(PKBParserIntegrationFollowsParent)
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
				4.		d = 7; } 
				5.	if a then { 
				6.		while e { 
				7.			c = 4; }} 
					else { 
				8.		d = 1; } 
				9.	e = 1; }

				procedure b{
				10.	a = b;
				11.	while a {
				12.		while b {
				13.			a = 1; }}
			
			*/
			testString = "procedure a {a = 7; while c { b = a; d = 7; } if a then { while e { c = 4; } } else { d = 1; } e = 1; } procedure b{ a = b; while a { while b { a = 1; }} }";
			pkb = parser.Parse(simpleSource, pkb, true, testString);
		}

		TEST_METHOD(PKBParserGetFollowsBeforeInvalid)
		{
			Assert::AreEqual(0, pkb.getFollowsBefore(1));
		}

		TEST_METHOD(PKBParserGetFollowsBefore)
		{
			Assert::AreEqual(1, pkb.getFollowsBefore(2));
		}

		TEST_METHOD(PKBParserGetFollowsAfter)
		{
			Assert::AreEqual(2, pkb.getFollowsAfter(1));
		}

		TEST_METHOD(PKBParserGetFollowsAfterInvalid)
		{
			Assert::AreEqual(0, pkb.getFollowsAfter(13));
		}


		TEST_METHOD(PKBParserGetFollowsBeforeStarInvalid)
		{
			std::vector<int> data = { };
			Assert::AreEqual(true, (data == pkb.getFollowsBeforeStar(1)));
		}

		TEST_METHOD(PKBParserGetFollowsBeforeStar)
		{
			std::vector<int> data = { 1,2,5 };
			Assert::AreEqual(true, (data == pkb.getFollowsBeforeStar(9)));
		}

		TEST_METHOD(PKBParserGetFollowsAfterStar)
		{
			std::vector<int> data = { 5, 9 };
			Assert::AreEqual(true, (data == pkb.getFollowsAfterStar(2)));
		}

		TEST_METHOD(PKBParserGetFollowsAfterStarInvalid)
		{
			std::vector<int> data = { };
			Assert::AreEqual(true, (data == pkb.getFollowsAfterStar(9)));
		}

		TEST_METHOD(PKBParserCheckFollows)
		{
			Assert::AreEqual(true, pkb.checkFollows(1, 2));
		}

		TEST_METHOD(PKBParserCheckFollowsInvalid)
		{
			Assert::AreEqual(false, pkb.checkFollows(2, 3));
		}

		TEST_METHOD(PKBParserCheckFollowsStar)
		{
			Assert::AreEqual(true, pkb.checkFollowsStar(1, 9));
		}

		TEST_METHOD(PKBParserCheckFollowsStarInvalid)
		{
			Assert::AreEqual(false, pkb.checkFollowsStar(9, 2));
		}

		TEST_METHOD(PKBParserGetAllFollows)
		{
			std::vector <std::vector<int>> output = pkb.getAllFollows();
			std::vector <std::vector<int>> data = { {3,4},{ 1,2 },{ 2,5 },{ 5,9 } ,{10,11} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBParserGetAllFollowsStar)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1, {2, 5, 9} });
			data.insert({ 2,{ 5, 9 } });
			data.insert({ 3,{ 4 } });
			data.insert({ 5,{ 9 } });
			data.insert({ 10,{ 11 } });
			unordered_map<int, std::vector<int>> output = pkb.getAllFollowsStar();
			for (auto it = output.begin(); it != output.end(); ++it) {
				Assert::AreEqual(true, (data[it->first] == it->second));
			}
		}

		TEST_METHOD(PKBParserGetParentInvalid)
		{
			Assert::AreEqual(0, pkb.getParent(1));
		}

		TEST_METHOD(PKBParserGetParent)
		{
			Assert::AreEqual(2, pkb.getParent(3));
		}

		TEST_METHOD(PKBParserGetChildren)
		{
			std::vector<int> data = {3, 4};
			Assert::AreEqual(true, (data == pkb.getChildren(2)));
		}

		TEST_METHOD(PKBParserGetChildrenInvalid)
		{
			std::vector<int> data = { };
			Assert::AreEqual(true, (data == pkb.getChildren(1)));
		}


		TEST_METHOD(PKBParserGetParentStarInvalid)
		{
			std::vector<int> data = {};
			Assert::AreEqual(true, (data == pkb.getParentStar(1)));
		}

		TEST_METHOD(PKBParserGetParentStar)
		{
			std::vector<int> data = { 6, 5 };
			Assert::AreEqual(true, (data == pkb.getParentStar(7)));
		}

		TEST_METHOD(PKBParserGetChildrenStar)
		{
			std::vector<int> data = { 6, 7, 8 };
			Assert::AreEqual(true, (data == pkb.getChildrenStar(5)));
		}

		TEST_METHOD(PKBParserGetChildrenStarInvalid)
		{
			std::vector<int> data = {};
			Assert::AreEqual(true, (data == pkb.getChildrenStar(1)));
		}

		TEST_METHOD(PKBParserCheckParent)
		{
			Assert::AreEqual(true, pkb.checkParent(2, 3));
		}

		TEST_METHOD(PKBParserCheckParentInvalid)
		{
			Assert::AreEqual(false, pkb.checkParent(1, 2));
		}

		TEST_METHOD(PKBParserCheckParentStar)
		{
			Assert::AreEqual(true, pkb.checkParentStar(5, 7));
		}

		TEST_METHOD(PKBParserCheckParentStarInvalid)
		{
			Assert::AreEqual(false, pkb.checkParentStar(7, 5));
		}

		TEST_METHOD(PKBParserGetAllParent)
		{
			std::vector <std::vector<int>> output = pkb.getAllParent();
			std::vector <std::vector<int>> data = { { 2, 3 },{ 2, 4 },{ 5, 6 },{ 6, 7 } ,{ 5, 8 } , {11,12}, {12,13} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBParserGetAllParentStar)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 2,{ 3, 4 } });
			data.insert({ 5,{ 6, 7, 8 } });
			data.insert({ 6,{ 7 } });
			data.insert({ 11,{ 12,13 } });
			data.insert({ 12,{ 13 } });
			unordered_map<int, std::vector<int>> output = pkb.getAllParentStar();
			for (auto it = output.begin(); it != output.end(); ++it) {
				Assert::AreEqual(true, (data[it->first] == it->second));
			}
		}

		TEST_METHOD(PKBParserGetUsesVariablesFromStatement)
		{
			std::vector<int> data = {2};
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
		}

		TEST_METHOD(PKBParserGetStatementsFromUsesVariables)
		{
			std::vector<int> data = { 3,5,11 };
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBParserGetUsesVariablesFromProcedure)
		{
			std::vector<int> data = { 1,2,3,4,5 };
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBParserGetProceduresFromUsesVariable)
		{
			std::vector<int> data = { 1,2 };
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBParserGetAllStatementUsesVariables)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1, {2} });
			data.insert({ 2,{ 3 } });
			data.insert({ 3,{ 1 } });
			data.insert({ 5,{ 1 } });
			data.insert({ 6,{ 5 } });
			data.insert({ 10,{ 2 } });
			data.insert({ 11,{ 1 } });
			data.insert({ 12,{ 2 } });
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == data));
		}

		TEST_METHOD(PKBParserGetAllVariableUsesStatements)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 3,5,11 } });
			data.insert({ 2,{ 1,10,12 } });
			data.insert({ 3,{ 2 } });
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == data));
		}

		TEST_METHOD(PKBParserGetAllProcedureUsesVariables)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1,2,3,5 } });
			data.insert({ 2,{ 1,2 } });
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == data));
		}

		TEST_METHOD(PKBParserGetAllVariableUsesProcedures)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1,2 } });
			data.insert({ 2,{ 1,2 } });
			data.insert({ 3,{ 1 } });
			data.insert({ 5,{ 1 } });
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == data));
		}

		TEST_METHOD(PKBParserCheckStatementUsesVariable)
		{
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 3));
		}

		TEST_METHOD(PKBParserCheckProcedureUsesVariable)
		{
			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(2, 3));
		}

		TEST_METHOD(PKBParserGetModifiesVariablesFromStatement)
		{
			std::vector<int> data = { 1 };
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
		}

		TEST_METHOD(PKBParserGetStatementsFromModifiesVariables)
		{
			std::vector<int> data = { 1,10 };
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBParserGetModifiesVariablesFromProcedure)
		{
			std::vector<int> data = { 1,2,3,4,5 };
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBParserGetProceduresFromModifiesVariable)
		{
			std::vector<int> data = { 1,2 };
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBParserGetAllStatementModifiesVariables)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1 } });
			data.insert({ 3,{ 2 } });
			data.insert({ 4,{ 4 } });
			data.insert({ 7,{ 3 } });
			data.insert({ 8,{ 4 } });
			data.insert({ 9,{ 5 } });
			data.insert({ 10,{ 1 } });
			data.insert({ 13,{ 1 } });
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == data));
		}

		TEST_METHOD(PKBParserGetAllVariableModifiesStatements)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1,10,13 } });
			data.insert({ 2,{ 3 } });
			data.insert({ 3,{ 7 } });
			data.insert({ 4,{ 4, 8 } });
			data.insert({ 5,{ 9 } });
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == data));
		}

		TEST_METHOD(PKBParserGetAllProcedureModifiesVariables)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1,2,3,4,5 } });
			data.insert({ 2,{ 1 } });
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == data));
		}

		TEST_METHOD(PKBParserGetAllVariableModifiesProcedures)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1,{ 1,2 } });
			data.insert({ 2,{ 1 } });
			data.insert({ 3,{ 1 } });
			data.insert({ 4,{ 1 } });
			data.insert({ 5,{ 1 } });
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == data));
		}

		TEST_METHOD(PKBParserCheckStatementModifiesVariable)
		{
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 2));
		}

		TEST_METHOD(PKBParserCheckProcedureModifiesVariable)
		{
			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 2));
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(2, 2));
		}

		TEST_METHOD(PKBParserGetStatementsWithConstant)
		{
			std::vector<int> data = { 8,9,13 };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
		}
	};
}