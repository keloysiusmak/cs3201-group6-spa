#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/QueryEvaluator.h";
#include "../SPA/QueryObject.h";
#include "../SPA/Pattern.h";
#include "../SPA/Clause.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
//
//namespace PKBEvaluatorIntegrationTesting
//{
//	PKB pkb;
//	Evaluator evaluator;
//	list<string> _variableID;
//
//	TEST_CLASS(PKBEvaluatorIntegration)
//	{
//	public:
//		/*
//		procedure a{
//		1.	a = b;
//		2.	while c {
//		3.		b = a;
//		4.		d = 7; }
//		5.	if a then {
//		6.		while e {
//		7.			c = 4; }}
//		else {
//		8.		d = 1; }
//		9.	e = 1; }
//
//		procedure b{
//		10.	a = b;
//		11.	while a {
//		12.		while b {
//		13.			a = 1; }}
//		*/
//
//		TEST_CLASS_INITIALIZE(setup)
//		{
//			pkb.insertToTable(1, 1, { {1}, {2}, {1}, {1} } );
//			pkb.insertToTable(1, 2, { { 1,2 },{ 3,1 },{ 2,4 },{ 2 } } );
//			pkb.insertToTable(1, 3,{ { 2 },{ 1 },{2},{ 1 } } );
//			pkb.insertToTable(1, 4, { { 2 },{  },{ 4 },{ 1 } } );
//			pkb.insertToTable(1, 5, { { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } } );
//			pkb.insertToTable(1, 6, { { 3,4 },{ 5 },{ 3 },{ 2 } } );
//			pkb.insertToTable(1, 7, { { 4 },{  },{ 3 },{ 1 } } );
//			pkb.insertToTable(1, 8, { { 5 }, {},{ 4 },{ 1 } } );
//			pkb.insertToTable(1, 9, { { 1 },{},{ 5 },{ 1 } } );
//			pkb.insertToTable(1, 10, { { 6 },{2},{ 1},{ 1 } } );
//			pkb.insertToTable(1, 11, { { 6,7 },{ 1,2 },{ 1 },{ 2 } } );
//			pkb.insertToTable(1, 12, { { 7,8 },{ 2 },{ 1 },{ 2 } } );
//			pkb.insertToTable(1, 13, { { 8 },{ },{1 },{ 1 } } );
//
//			pkb.insertToTable(2, 1, { {0},{1,2,5,9}, {1}} );
//			pkb.insertToTable(2, 2, { { 2 },{ 3,4 },{ 2 } } );
//			pkb.insertToTable(2, 3, { { 5 },{ 6 },{ 3 } } );
//			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } } );
//			pkb.insertToTable(2, 5, { { 5 },{ 8 },{ 3 } } );
//			pkb.insertToTable(2, 6, { { 0 },{ 10,11 },{ 2 } } );
//			pkb.insertToTable(2, 7, { { 11 },{ 12 },{ 2 } } );
//			pkb.insertToTable(2, 8, { { 12 },{ 13 },{ 2 } } );
//
//			pkb.insertToTable(3, 1, {{1},{1,2,3,5},{1,2,3,4,5}} );
//			pkb.insertToTable(3, 2, { { 6 },{ 1,2 },{ 1 } } );
//
//			pkb.insertToTable(4, 1, {{2,3,5,11}, {1,2}} );
//			pkb.insertToTable(4, 2, { {1,10,11, 12},{ 1,2 } } );
//			pkb.insertToTable(4, 3, { { 2 },{ 1 } } );
//			pkb.insertToTable(4, 5, { {5,6},{1} } );
//
//			pkb.insertToTable(5, 1, { {1,10,11,12,13},{1,2} } );
//			pkb.insertToTable(5, 2, { { 2,3 },{ 1 } } );
//			pkb.insertToTable(5, 3, { { 5,6,7 },{ 1 } } );
//			pkb.insertToTable(5, 4, { { 5,8 },{ 1 } } );
//			pkb.insertToTable(5, 5, { { 9 },{ 1 } } );
//
//			pkb.insertToTable(7, 1, {{8, 9, 13}} );
//			pkb.insertToTable(7, 4, { { 7 } } );
//			pkb.insertToTable(7, 7, { { 4 } } );
//
//			pkb.insertToNameTable(8, "a");
//			pkb.insertToNameTable(8, "b");
//
//			pkb.insertToNameTable(9, "a");
//			pkb.insertToNameTable(9, "b");
//			pkb.insertToNameTable(9, "c");
//			pkb.insertToNameTable(9, "d");
//			pkb.insertToNameTable(9, "e");
//
//			evaluator.setPKB(pkb);
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementFollowsBefore)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Follows, STMT, "s", INTEGER, "2");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Follows, STMT, "s", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementFollowsAfter)
//		{
//			QueryObject q;
//			
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Follows, INTEGER, "1", STMT, "s");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Follows, INTEGER, "9", STMT, "s");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorProgLineFollowsBefore)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(PROG_LINE, "n");
//			q.insertClause(Follows, PROG_LINE, "n", INTEGER, "2");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(PROG_LINE, "n");
//			q1.insertClause(Follows, PROG_LINE, "n", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorProgLineFollowsAfter)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(PROG_LINE, "n");
//			q.insertClause(Follows, INTEGER, "1", PROG_LINE, "n");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(PROG_LINE, "n");
//			q1.insertClause(Follows, INTEGER, "9", PROG_LINE, "n");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileFollowsBefore)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Follows, WHILE, "w", INTEGER, "5");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Follows, WHILE, "w", INTEGER, "2");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileFollowsAfter)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Follows, INTEGER, "1", WHILE, "w");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Follows, INTEGER, "5", WHILE, "w");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfFollowsBefore)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Follows, IF, "ifs", INTEGER, "9");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Follows, IF, "ifs", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfFollowsAfter)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Follows, INTEGER, "2", IF, "ifs");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Follows, INTEGER, "9", IF, "ifs");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementFollowsBeforeStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(FollowsT, STMT, "s", INTEGER, "9");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("2");
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(FollowsT, STMT, "s", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementFollowsAfterStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(PROG_LINE, "n");
//			q.insertClause(FollowsT, INTEGER, "1", PROG_LINE, "n");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			expected.push_back("5");
//			expected.push_back("9");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorProgLineFollowsBeforeStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(PROG_LINE, "n");
//			q.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "9");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("2");
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(PROG_LINE, "n");
//			q1.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorProgLineFollowsAfterStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(FollowsT, INTEGER, "1", STMT, "s");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			expected.push_back("5");
//			expected.push_back("9");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(PROG_LINE, "n");
//			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileFollowsBeforeStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(FollowsT, WHILE, "w", INTEGER, "9");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(FollowsT, WHILE, "w", INTEGER, "2");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileFollowsAfterStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(FollowsT, INTEGER, "1", WHILE, "w");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(FollowsT, INTEGER, "9", WHILE, "w");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfFollowsBeforeStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(FollowsT, IF, "ifs", INTEGER, "9");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(FollowsT, IF, "ifs", INTEGER, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfFollowsAfterStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(FollowsT, INTEGER, "1", IF, "ifs");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(FollowsT, INTEGER, "9", IF, "ifs");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorChildren)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Parent, INTEGER, "2", STMT, "s");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("3");
//			expected.push_back("4");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Parent, INTEGER, "5", STMT, "s");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("6");
//			expected.push_back("8");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(Parent, INTEGER, "11", STMT, "s");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("12");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q3;
//
//			q3.insertSelectStmt(STMT, "s");
//			q3.insertClause(Parent, INTEGER, "12", STMT, "s");
//
//			evaluator.setQueryObject(q3);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParent)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Parent, STMT, "s", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Parent, STMT, "s", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(Parent, STMT, "s", INTEGER, "9");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParentIf)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Parent, IF, "ifs", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Parent, IF, "ifs", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(IF, "ifs");
//			q2.insertClause(Parent, IF, "ifs", INTEGER, "8");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q3;
//
//			q3.insertSelectStmt(IF, "ifs");
//			q3.insertClause(Parent, IF, "ifs", INTEGER, "7");
//
//			evaluator.setQueryObject(q3);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParentWhile)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Parent, WHILE, "w", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Parent, WHILE, "w", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(WHILE, "w");
//			q2.insertClause(Parent, WHILE, "w", INTEGER, "1");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorChildrenStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(ParentT, INTEGER, "2", STMT, "s");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("3");
//			expected.push_back("4");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(ParentT, INTEGER, "5", STMT, "s");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("6");
//			expected.push_back("7");
//			expected.push_back("8");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(ParentT, INTEGER, "11", STMT, "s");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("12");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q3;
//
//			q3.insertSelectStmt(STMT, "s");
//			q3.insertClause(ParentT, INTEGER, "12", STMT, "s");
//
//			evaluator.setQueryObject(q3);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q4;
//
//			q4.insertSelectStmt(STMT, "s");
//			q4.insertClause(ParentT, INTEGER, "13", STMT, "s");
//
//			evaluator.setQueryObject(q4);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParentStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Parent, STMT, "s", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Parent, STMT, "s", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(Parent, STMT, "s", INTEGER, "10");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParentIfStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Parent, IF, "ifs", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Parent, IF, "ifs", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(IF, "ifs");
//			q2.insertClause(Parent, IF, "ifs", INTEGER, "7");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorParentWhileStar)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Parent, WHILE, "w", INTEGER, "3");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Parent, WHILE, "w", INTEGER, "6");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(WHILE, "w");
//			q2.insertClause(Parent, WHILE, "w", INTEGER, "8");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementNumberUsesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(VARIABLE, "v");
//			q.insertClause(Uses, INTEGER, "1", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("b");
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementUsesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Uses, STMT, "s", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("2");
//			expected.push_back("3");
//			expected.push_back("5");
//			expected.push_back("6");
//			expected.push_back("10");
//			expected.push_back("11");
//			expected.push_back("12");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Uses, STMT, "s", IDENT, "b");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("1");
//			expected.push_back("10");
//			expected.push_back("11");
//			expected.push_back("12");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(Uses, STMT, "s", IDENT, "f");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfUsesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Uses, IF, "ifs", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Uses, IF, "ifs", IDENT, "a");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(IF, "ifs");
//			q2.insertClause(Uses, IF, "ifs", IDENT, "b");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileUsesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Uses, WHILE, "w", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			expected.push_back("6");
//			expected.push_back("11");
//			expected.push_back("12");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Uses, WHILE, "w", IDENT, "c");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(WHILE, "w");
//			q2.insertClause(Uses, WHILE, "w", IDENT, "d");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorAssignUsesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertClause(Uses, ASSIGN, "a", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("3");
//			expected.push_back("10");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertClause(Uses, ASSIGN, "a", IDENT, "a");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("3");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(ASSIGN, "a");
//			q2.insertClause(Uses, ASSIGN, "a", IDENT, "e");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//
//		TEST_METHOD(PKBEvaluatorStatementNumberModifiesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(VARIABLE, "v");
//			q.insertClause(Modifies, INTEGER, "1", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("a");
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorStatementModifiesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(STMT, "s");
//			q.insertClause(Modifies, STMT, "s", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("2");
//			expected.push_back("3");
//			expected.push_back("4");
//			expected.push_back("5");
//			expected.push_back("6");
//			expected.push_back("7");
//			expected.push_back("8");
//			expected.push_back("9");
//			expected.push_back("10");
//			expected.push_back("11");
//			expected.push_back("12");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(STMT, "s");
//			q1.insertClause(Modifies, STMT, "s", IDENT, "b");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("2");
//			expected.push_back("3");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(STMT, "s");
//			q2.insertClause(Modifies, STMT, "s", IDENT, "f");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorIfModifiesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(IF, "ifs");
//			q.insertClause(Modifies, IF, "ifs", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(IF, "ifs");
//			q1.insertClause(Modifies, IF, "ifs", IDENT, "c");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("5");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(IF, "ifs");
//			q2.insertClause(Modifies, IF, "ifs", IDENT, "b");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorWhileModifiesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(WHILE, "w");
//			q.insertClause(Modifies, WHILE, "w", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("2");
//			expected.push_back("6");
//			expected.push_back("11");
//			expected.push_back("12");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(WHILE, "w");
//			q1.insertClause(Modifies, WHILE, "w", IDENT, "b");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("2");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(WHILE, "w");
//			q2.insertClause(Modifies, WHILE, "w", IDENT, "d");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorAssignModifiesVariable)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertClause(Modifies, ASSIGN, "a", VARIABLE, "v");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("3");
//			expected.push_back("4");
//			expected.push_back("7");
//			expected.push_back("8");
//			expected.push_back("9");
//			expected.push_back("10");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertClause(Modifies, ASSIGN, "a", IDENT, "a");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			expected.push_back("1");
//			expected.push_back("10");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(ASSIGN, "a");
//			q2.insertClause(Modifies, ASSIGN, "a", IDENT, "f");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorPatternAllVarName)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertPattern(ASSIGN, "a", ALL, "_", VAR_NAME, "b");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("10");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertPattern(ASSIGN, "a", ALL, "_", VAR_NAME, "f");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorPatternAllConstant)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertPattern(ASSIGN, "a", ALL, "_", CONSTANT, "1");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("8");
//			expected.push_back("9");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertPattern(ASSIGN, "a", ALL, "_", CONSTANT, "17");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorPatternIdentAll)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertPattern(ASSIGN, "a", IDENT, "a", ALL, "_");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("10");
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertPattern(ASSIGN, "a", IDENT, "f", ALL, "_");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorPatternIdentVarName)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertPattern(ASSIGN, "a", IDENT, "a", VAR_NAME, "b");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("1");
//			expected.push_back("10");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertPattern(ASSIGN, "a", IDENT, "f", VAR_NAME, "b");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(ASSIGN, "a");
//			q2.insertPattern(ASSIGN, "a", IDENT, "a", VAR_NAME, "f");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//		TEST_METHOD(PKBEvaluatorPatternIdentConstant)
//		{
//			QueryObject q;
//
//			q.insertSelectStmt(ASSIGN, "a");
//			q.insertPattern(ASSIGN, "a", IDENT, "a", CONSTANT, "1");
//
//			evaluator.setQueryObject(q);
//
//			list<string> result = evaluator.evaluateQuery();
//			list<string> expected;
//			expected.push_back("13");
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q1;
//
//			q1.insertSelectStmt(ASSIGN, "a");
//			q1.insertPattern(ASSIGN, "a", IDENT, "f", CONSTANT, "1");
//
//			evaluator.setQueryObject(q1);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//
//			QueryObject q2;
//
//			q2.insertSelectStmt(ASSIGN, "a");
//			q2.insertPattern(ASSIGN, "a", IDENT, "a", CONSTANT, "13");
//
//			evaluator.setQueryObject(q2);
//
//			result = evaluator.evaluateQuery();
//			expected.clear();
//			Assert::AreEqual(true, (expected == result));
//		}
//
//	};
//}