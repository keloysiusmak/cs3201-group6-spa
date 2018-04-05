#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SPA/PKB.h";
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
				3.		b = (a+1)*4;
				4.		call b; } 
				5.	if a then { 
				6.		while e { 
				7.			c = 4; }} 
					else { 
				8.		d = 1; } 
				9.	e = 1; }

				procedure b{
				10.	f = g;
				11.	while h {
				12.		while i {
				13.			j = 1; }}}
			
			*/
			testString = "procedure a {a = b; while c { b = (a + 1 )* 4; call b; } if a then { while e { c = 4; } } else { d = 1; } e = 1; } procedure b{ f = g; while h { while i { j = 1; }} }";
			testString = "procedure Main {while one {seven = two + three * four * five * six;four = four + 3;three = three - one * ( two + ten);eight = three + 1; }one = 5;two = one;call Third;three = two + 2;four = 1;call Second;one = two + one + three + five +5;while eight {one = three + 3 + 2+two;four = four + 7;if eight then{ten = five + 10; }else {three = one + 1;while three {two = three + one * ten - two;if eight then {if five then {eleven = five + 1;twelve = thirteen * five * five;while ten {call Second;}fourteen = 2 * thirteen * five - seven + eight;}else{twelve = night * ten;fifteen = 2 * night + ten;}}else{three = one + 3 * ( two - nine);}}while two{en = five - 500;night = two + three * ( three * three - ( three + three ) );}six = fourteen * 3;one = one + two + three;}one = one + two + three + seven * five;while eight {eight = eight + 1 * five * fifteen;}}one = one + sixteen ;}procedure Second {thirteen = 2;eleven = twelve - thirteen;while one {call Third;}if two then {one = two + three * ten;}else {call Third;one = two;while three {one = two;}}}procedure Third {if seven then {fifteen = sixteen - six + six;}else {seven = three * ten; }}";
			pkb = parser.Parse(simpleSource, pkb, true, testString);
			de = DesignExtractor();
			de.extract(pkb);
			int a = 1;
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
			data.push_back({ 6,7 });
			data.push_back({ 5,6 });
			data.push_back({ 5,8 });
			data.push_back({ 5,7 });
			data.push_back({ 12,13 });
			data.push_back({ 11,12 });
			data.push_back({ 11,13 });
			data.push_back({ 2,3 });
			data.push_back({ 2,4 });
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
			std::vector<std::vector<int>> data = { {2},{3},{5} };
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
		}

		TEST_METHOD(PKBDEParserGetUsesVariablesFromProcedure)
		{
			std::vector<std::vector<int>> data = { {1},{2},{3},{4}, {7}, {8}, {9} };
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBDEParserGetProceduresFromUsesVariable)
		{
			std::vector<std::vector<int>> data = { {1},{2} };
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(9) == data));
		}

		TEST_METHOD(PKBDEParserGetAllStatementUsesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			data.push_back({ 3,1 });
			data.push_back({ 6,4 });
			data.push_back({ 5,1 });
			data.push_back({ 5,4 });
			data.push_back({ 10,7 });
			data.push_back({ 12,9 });
			data.push_back({ 11,8 });
			data.push_back({ 11,9 });
			data.push_back({ 4,7 });
			data.push_back({ 4,8 });
			data.push_back({ 4,9 });
			data.push_back({ 2,1 });
			data.push_back({ 2,3 });
			data.push_back({ 2,7 });
			data.push_back({ 2,8 });
			data.push_back({ 2,9 });
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableUsesStatements)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 9,2 });
			data.push_back({ 9,4 });
			data.push_back({ 9,11 });
			data.push_back({ 9,12 });
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,5 });
			data.push_back({ 2,1 });
			data.push_back({ 3,2 });
			data.push_back({ 4,5 });
			data.push_back({ 4,6 });
			data.push_back({ 7,2 });
			data.push_back({ 7,4 });
			data.push_back({ 7,10 });
			data.push_back({ 8,2 });
			data.push_back({ 8,4 });
			data.push_back({ 8,11 });
			vector<vector<int>> a = pkb.getAllVariableUsesStatements();

			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == data));
		}

		TEST_METHOD(PKBDEParserGetAllProcedureUsesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,4 });
			data.push_back({ 1,7 });
			data.push_back({ 1,8 });
			data.push_back({ 1,9 });
			data.push_back({ 2,7 });
			data.push_back({ 2,8 });
			data.push_back({ 2,9 });
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableUsesProcedures)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 9,1 });
			data.push_back({ 9,2 });
			data.push_back({ 1,1 });
			data.push_back({ 2,1 });
			data.push_back({ 3,1 });
			data.push_back({ 4,1 });
			data.push_back({ 7,1 });
			data.push_back({ 7,2 });
			data.push_back({ 8,1 });
			data.push_back({ 8,2 });

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
			std::vector<std::vector<int>> data = { {2},{4},{11},{12},{13} };
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(10) == data));
		}

		TEST_METHOD(PKBDEParserGetModifiesVariablesFromProcedure)
		{
			std::vector<std::vector<int>> data = { {1},{2},{3},{4},{5}, {6},{10} };
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
		}

		TEST_METHOD(PKBDEParserGetProceduresFromModifiesVariable)
		{
			std::vector<std::vector<int>> data = { {1},{2} };
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(6) == data));
		}

		TEST_METHOD(PKBDEParserGetAllStatementModifiesVariables)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 3,2 });
			data.push_back({ 6,3 });
			data.push_back({ 7,3 });
			data.push_back({ 5,3 });
			data.push_back({ 5,5 });
			data.push_back({ 8,5 });
			data.push_back({ 9,4 });
			data.push_back({ 10,6 });
			data.push_back({ 12,10 });
			data.push_back({ 11,10 });
			data.push_back({ 13,10 });
			data.push_back({ 4,6 });
			data.push_back({ 4,10 });
			data.push_back({ 2,2 });
			data.push_back({ 2,6 });
			data.push_back({ 2,10 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableModifiesStatements)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 2,2 });
			data.push_back({ 2,3 });
			data.push_back({ 3,5 });
			data.push_back({ 3,6 });
			data.push_back({ 3,7 });
			data.push_back({ 4,9 });
			data.push_back({ 5,5 });
			data.push_back({ 5,8 });
			data.push_back({ 6,2 });
			data.push_back({ 6,4 });
			data.push_back({ 6,10 });
			data.push_back({ 10,2 });
			data.push_back({ 10,4 });
			data.push_back({ 10,11 });
			data.push_back({ 10,12 });
			data.push_back({ 10,13 });

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
			data.push_back({ 1,6 });
			data.push_back({ 1,10 });
			data.push_back({ 2,6 });
			data.push_back({ 2,10 });
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == data));
		}

		TEST_METHOD(PKBDEParserGetAllVariableModifiesProcedures)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,1 });
			data.push_back({ 2,1 });
			data.push_back({ 3,1 });
			data.push_back({ 4,1 });
			data.push_back({ 5,1 });
			data.push_back({ 6,1 });
			data.push_back({ 6,2 });
			data.push_back({ 10,1 });
			data.push_back({ 10,2 });
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

		TEST_METHOD(PKBDEParserGetNextBefore)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 5 });
			data.push_back({ 7 });
			Assert::AreEqual(true, (pkb.getNextBefore(6) == data));
		}

		TEST_METHOD(PKBDEParserGetNextAfter)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 7 });
			data.push_back({ 9 });
			Assert::AreEqual(true, (pkb.getNextAfter(6) == data));
		}

		TEST_METHOD(PKBDEParserGetNextBeforeStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1 });
			data.push_back({ 2 });
			data.push_back({ 3 });
			data.push_back({ 4 });
			Assert::AreEqual(true, (pkb.getNextBeforeStar(5) == data));
		}

		TEST_METHOD(PKBDEParserGetNextAfterStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 6 });
			data.push_back({ 7 });
			data.push_back({ 8 });
			data.push_back({ 9 });
			Assert::AreEqual(true, (pkb.getNextAfterStar(5) == data));
		}

		TEST_METHOD(PKBDEParserGetAllNext)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			data.push_back({ 10,11 });
			data.push_back({ 2,3 });
			data.push_back({ 2,5 });
			data.push_back({ 3,4 });
			data.push_back({ 4,2 });
			data.push_back({ 6,7 });
			data.push_back({ 6,9 });
			data.push_back({ 7,6 });
			data.push_back({ 5,6 });
			data.push_back({ 5,8 });
			data.push_back({ 8,9 });
			data.push_back({ 11,12 });
			data.push_back({ 12,11 });
			data.push_back({ 12,13 });
			data.push_back({ 13,12 });
			Assert::AreEqual(true, (pkb.getAllNext() == data));
		}

		TEST_METHOD(PKBDEParserGetAllNextStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,4 });
			data.push_back({ 1,5 });
			data.push_back({ 1,6 });
			data.push_back({ 1,7 });
			data.push_back({ 1,8 });
			data.push_back({ 1,9 });
			data.push_back({ 3,2 });
			data.push_back({ 3,3 });
			data.push_back({ 3,4 });
			data.push_back({ 3,5 });
			data.push_back({ 3,6 });
			data.push_back({ 3,7 });
			data.push_back({ 3,8 });
			data.push_back({ 3,9 });
			data.push_back({ 6,6 });
			data.push_back({ 6,7 });
			data.push_back({ 6,9 });
			data.push_back({ 7,6 });
			data.push_back({ 7,7 });
			data.push_back({ 7,9 });
			data.push_back({ 5,6 });
			data.push_back({ 5,7 });
			data.push_back({ 5,8 });
			data.push_back({ 5,9 });
			data.push_back({ 8,9 });
			data.push_back({ 10,11 });
			data.push_back({ 10,12 });
			data.push_back({ 10,13 });
			data.push_back({ 12,11 });
			data.push_back({ 12,12 });
			data.push_back({ 12,13 });
			data.push_back({ 11,11 });
			data.push_back({ 11,12 });
			data.push_back({ 11,13 });
			data.push_back({ 13,11 });
			data.push_back({ 13,12 });
			data.push_back({ 13,13 });
			data.push_back({ 4,2 });
			data.push_back({ 4,3 });
			data.push_back({ 4,4 });
			data.push_back({ 4,5 });
			data.push_back({ 4,6 });
			data.push_back({ 4,7 });
			data.push_back({ 4,8 });
			data.push_back({ 4,9 });
			data.push_back({ 2,2 });
			data.push_back({ 2,3 });
			data.push_back({ 2,4 });
			data.push_back({ 2,5 });
			data.push_back({ 2,6 });
			data.push_back({ 2,7 });
			data.push_back({ 2,8 });
			data.push_back({ 2,9 });
			Assert::AreEqual(true, (pkb.getAllNextStar() == data));
		}

		TEST_METHOD(PKBDEParserGetCallsBefore)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1 });
			Assert::AreEqual(true, (pkb.getCallsBefore(2) == data));
		}

		TEST_METHOD(PKBDEParserGetCallsAfter)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 2 });
			Assert::AreEqual(true, (pkb.getCallsAfter(1) == data));
		}

		TEST_METHOD(PKBDEParserGetCallsBeforeStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1 });
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(2) == data));
		}

		TEST_METHOD(PKBDEParserGetCallsAfterStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 2 });
			Assert::AreEqual(true, (pkb.getCallsAfterStar(1) == data));
		}

		TEST_METHOD(PKBDEParserGetAllCalls)
		{
			std::vector<std::vector<int>> data;
			data.push_back({1,2});
			Assert::AreEqual(true, (pkb.getAllCalls() == data));
		}

		TEST_METHOD(PKBDEParserGetAllCallsStar)
		{
			std::vector<std::vector<int>> data;
			data.push_back({ 1,2 });
			Assert::AreEqual(true, (pkb.getAllCallsStar() == data));
		}

		TEST_METHOD(PKBDEParserGetCallStatementsCallingProcedure)
		{
			std::vector<std::vector<int>> data;
			data = { { 4 } };
			Assert::AreEqual(true, (pkb.getCallStatementsCallingProcedure(2) == data));

		}

		TEST_METHOD(PKBDEParserGetProcedureCalledByCallStatement)
		{
			std::vector<std::vector<int>> data;
			data = { { 2 } };
			Assert::AreEqual(true, (pkb.getProcedureCalledByCallStatement(4) == data));

		}

		TEST_METHOD(PKBDEParserGetAllCallStatementsCallingProcedure)
		{
			std::vector<std::vector<int>> data;
			data = { { 2,4 } };
			Assert::AreEqual(true, (pkb.getAllCallStatementsCallingProcedure() == data));

		}

		TEST_METHOD(PKBDEParserGetAllProcedureCalledByCallStatement)
		{
			std::vector<std::vector<int>> data;
			data = { { 4,2 } };
			Assert::AreEqual(true, (pkb.getAllProcedureCalledByCallStatement() == data));

		}

		TEST_METHOD(PKBDEParserGetStatementsWithPattern)
		{
			Param ep;
			Param lp;
			Param rp;

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
			std::vector<std::vector<int>> expected = { { 1 },{ 3 },{ 7 }, {8}, {9}, {10},{13} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p1) == expected));

			//a(_,_"a+1"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p2(ep, lp, rp);
			expected = { {3} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p2) == expected));

			//a(_,"a+1")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p3(ep, lp, rp);
			expected.clear();
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
			expected = { { 1,1 },{ 3,2 },{ 7,3 },{ 8,5 },{ 9,4 },{ 10,6 },{ 13,10 } };
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

			//a(v,_"a+1"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p6(ep, lp, rp);
			expected = { { 3, 2 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p6) == expected));

			//a(v,"a+1")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p7(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p7) == expected));

			//a("b", _"a+1"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "b";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p8(ep, lp, rp);
			expected = { { 3 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p8) == expected));

			//a("b", "a+1")
			ep.type = ASSIGN;
			ep.value = "A";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "B";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p9(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p9) == expected));

			//a(_, _"a+1"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p10(ep, lp, rp);
			expected = { { 3 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p10) == expected));

			//a("y", _"a+1"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "y";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "a|1|+|";
			rp.attribute = NONE;

			Pattern p11(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p11) == expected));

			//ifs(_,_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p12(ep, lp, rp);
			expected = { { 5} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p12) == expected));

			//ifs(v,_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p13(ep, lp, rp);
			expected = { { 5,1 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p13) == expected));

			//ifs("j",_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p14(ep, lp, rp);
			expected = { { 5 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p14) == expected));

			//ifs("k",_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "k";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p15(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p15) == expected));

			//w(_,_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p17(ep, lp, rp);
			expected = { { 6}, {12}, {11}, {2} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p17) == expected));

			//w(v,_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p18(ep, lp, rp);
			expected = {{ 6,4 },{ 12,9 },{ 11,8 },{ 2,3 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p18) == expected));

			//w("h",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "h";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p19(ep, lp, rp);
			expected = { { 11 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p19) == expected));

			//w("i",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "i";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p20(ep, lp, rp);
			expected = { { 12 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p20) == expected));

			//w("n",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "n";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p21(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p21) == expected));
		}

		TEST_METHOD(PKBDEParserGetStatementsWithConstant)
		{
			std::vector<std::vector<int>> data = { {3}, {8},{9},{13} };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
			data = { { 3 }, {7} };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(4) == data));
		}

		TEST_METHOD(PKBDEParserGetAllConstants)
		{
			std::vector<std::vector<int>> data = { { 4 }, {1} };
			Assert::AreEqual(true, (pkb.getAllConstants() == data));
		}
	};
}