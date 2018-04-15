#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SPA/PKB.h";
#include "../SPA/QueryEvaluator.h";
#include "../SPA/DesignExtractor.h";
#include "../SPA/QueryObject.h";
#include "../SPA/Pattern.h";
#include "../SPA/Clause.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBEvaluatorIntegrationTesting
{
	PKB pkb;
	QueryEvaluator evaluator;
	list<string> _variableID;
	DesignExtractor de;

	TEST_CLASS(PKBEvaluatorIntegration)
	{
	public:
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

		TEST_CLASS_INITIALIZE(setup)
		{
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1}, {2}, {1}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1,2 },{ 3,1 },{ 2,4 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1 },{2},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2 },{  },{ },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3,4 },{ 5 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 4 },{  },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 5 }, {},{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{ 5 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{2},{ 1},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 6,7 },{ 1,2 },{ 1 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 7,8 },{ 2 },{ 1 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ },{1 },{ 1 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0},{1,2,5,9}, {1} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3,4 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 5 },{ 6 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 5 },{ 8 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 0 },{ 10,11 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 11 },{ 12 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 13 },{ 2 } });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { {1},{1,2,3,5},{1,2,3,4,5} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 6 },{ 1,2 },{ 1 } });

			pkb.insertToTable(USES_TABLE, 1, { {2,3,5,11}, {1,2} });
			pkb.insertToTable(USES_TABLE, 2, { {1,10,11, 12},{ 1,2 } });
			pkb.insertToTable(USES_TABLE, 3, { { 2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 5, { {5,6},{1} });

			pkb.insertToTable(MODIFIES_TABLE, 1, { {1,10,11,12,13},{1,2} });
			pkb.insertToTable(MODIFIES_TABLE, 2, { { 2,3 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 3, { { 5,6,7 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 4, { { 5,8 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 5, { { 9 },{ 1 } });

			pkb.insertToTable(CONST_TABLE, 1, { {8, 9, 13} });
			pkb.insertToTable(CONST_TABLE, 4, { { 7 } });
			pkb.insertToTable(CONST_TABLE, 7, { { 4 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3,5 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6,8 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7,9 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 9 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 11 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 11,13 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 12 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5,7 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 9, { { 6, 8} });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 10,12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 11,13 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });

			pkb.insertToTable(CALLS_TABLE, 1, { { 2 }, {} });
			pkb.insertToTable(CALLS_TABLE, 2, { {  },{4} });

			pkb.insertToTable(CALLS_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2 } });
			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 2, { { 1 } });

			pkb.insertToTable(CALL_STATEMENT_TABLE, 4, { {2} });

			pkb.insertToNameTable(PATTERN_TABLE, { "a", "b|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "b", "a|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "e" });
			pkb.insertToNameTable(PATTERN_TABLE, { "c", "4|b|+|x|a|*|+" });
			pkb.insertToNameTable(PATTERN_TABLE, { "d", "1|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "e", "1|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "b|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "b" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "1|" });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });

			pkb.insertToTable(PROC_NAME_CALL_NAME_TABLE, 2, { {4} });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 4, { { 4 } });

			evaluator.setPKB(pkb);
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Follows, STMT, "s", INTEGER, "2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Follows, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsAfter)
		{
			QueryObject q;
			
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Follows, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Follows, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n", NONE);
			q.insertClause(Follows, PROG_LINE, "n", INTEGER, "2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n", NONE);
			q1.insertClause(Follows, PROG_LINE, "n", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n", NONE);
			q.insertClause(Follows, INTEGER, "1", PROG_LINE, "n", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n", NONE);
			q1.insertClause(Follows, INTEGER, "9", PROG_LINE, "n", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Follows, WHILE, "w", INTEGER, "5", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Follows, WHILE, "w", INTEGER, "2", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Follows, INTEGER, "1", WHILE, "w", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Follows, INTEGER, "5", WHILE, "w", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Follows, IF, "ifs", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Follows, IF, "ifs", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Follows, INTEGER, "2", IF, "ifs", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Follows, INTEGER, "9", IF, "ifs", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(FollowsT, STMT, "s", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(FollowsT, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n", NONE);
			q.insertClause(FollowsT, INTEGER, "1", PROG_LINE, "n", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("5");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n", NONE);
			q.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n", NONE);
			q1.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(FollowsT, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("5");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n", NONE);
			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(FollowsT, WHILE, "w", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(FollowsT, WHILE, "w", INTEGER, "2", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(FollowsT, INTEGER, "1", WHILE, "w", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(FollowsT, INTEGER, "9", WHILE, "w", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(FollowsT, IF, "ifs", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(FollowsT, IF, "ifs", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(FollowsT, INTEGER, "1", IF, "ifs", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(FollowsT, INTEGER, "9", IF, "ifs", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorChildren)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Parent, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("3");
			expected.push_back("4");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Parent, INTEGER, "5", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("6");
			expected.push_back("8");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Parent, INTEGER, "11", STMT, "s", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(STMT, "s", NONE);
			q3.insertClause(Parent, INTEGER, "12", STMT, "s", false);

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParent)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Parent, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Parent, STMT, "s", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Parent, STMT, "s", INTEGER, "9", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentIf)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Parent, IF, "ifs", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Parent, IF, "ifs", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs", NONE);
			q2.insertClause(Parent, IF, "ifs", INTEGER, "8", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(IF, "ifs", NONE);
			q3.insertClause(Parent, IF, "ifs", INTEGER, "7", false);

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentWhile)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Parent, WHILE, "w", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Parent, WHILE, "w", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w", NONE);
			q2.insertClause(Parent, WHILE, "w", INTEGER, "1", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorChildrenStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(ParentT, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("3");
			expected.push_back("4");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(ParentT, INTEGER, "5", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("6");
			expected.push_back("7");
			expected.push_back("8");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(ParentT, INTEGER, "11", STMT, "s", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("12");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(STMT, "s", NONE);
			q3.insertClause(ParentT, INTEGER, "12", STMT, "s", false);

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q4;

			q4.insertSelectStmt(STMT, "s", NONE);
			q4.insertClause(ParentT, INTEGER, "13", STMT, "s", false);

			evaluator.setQueryObject(q4);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Parent, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Parent, STMT, "s", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Parent, STMT, "s", INTEGER, "10", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentIfStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Parent, IF, "ifs", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Parent, IF, "ifs", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs", NONE);
			q2.insertClause(Parent, IF, "ifs", INTEGER, "7", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentWhileStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Parent, WHILE, "w", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Parent, WHILE, "w", INTEGER, "6", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w", NONE);
			q2.insertClause(Parent, WHILE, "w", INTEGER, "8", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementNumberUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(VARIABLE, "v", NONE);
			q.insertClause(Uses, INTEGER, "1", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("b");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Uses, STMT, "s", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("2");
			expected.push_back("3");
			expected.push_back("5");
			expected.push_back("6");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Uses, STMT, "s", VAR_IDENT, "b", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Uses, STMT, "s", VAR_IDENT, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Uses, IF, "ifs", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Uses, IF, "ifs", VAR_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs", NONE);
			q2.insertClause(Uses, IF, "ifs", VAR_IDENT, "b", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Uses, WHILE, "w", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("2");
			expected.push_back("6");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Uses, WHILE, "w", VAR_IDENT, "c", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w", NONE);
			q2.insertClause(Uses, WHILE, "w", VAR_IDENT, "d", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAssignUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertClause(Uses, ASSIGN, "a", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("3");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertClause(Uses, ASSIGN, "a", VAR_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("3");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a", NONE);
			q2.insertClause(Uses, ASSIGN, "a", VAR_IDENT, "e", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}


		TEST_METHOD(PKBEvaluatorProcedureUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(Uses, PROCEDURE, "p", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("a");
			expected.push_back("b");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(Uses, PROCEDURE, "p", VAR_IDENT, "e", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("a");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(PROCEDURE, "p", NONE);
			q2.insertClause(Uses, PROCEDURE, "p", VAR_IDENT, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();

			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementNumberModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(VARIABLE, "v", NONE);
			q.insertClause(Modifies, INTEGER, "1", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("a");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Modifies, STMT, "s", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("13");
			expected.push_back("2");
			expected.push_back("3");
			expected.push_back("5");
			expected.push_back("6");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Modifies, STMT, "s", VAR_IDENT, "b", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			expected.push_back("3");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Modifies, STMT, "s", VAR_IDENT, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs", NONE);
			q.insertClause(Modifies, IF, "ifs", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs", NONE);
			q1.insertClause(Modifies, IF, "ifs", VAR_IDENT, "c", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs", NONE);
			q2.insertClause(Modifies, IF, "ifs", VAR_IDENT, "b", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w", NONE);
			q.insertClause(Modifies, WHILE, "w", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("2");
			expected.push_back("6");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w", NONE);
			q1.insertClause(Modifies, WHILE, "w", VAR_IDENT, "b", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w", NONE);
			q2.insertClause(Modifies, WHILE, "w", VAR_IDENT, "d", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAssignModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertClause(Modifies, ASSIGN, "a", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("13");
			expected.push_back("3");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a", NONE);
			q2.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProcedureModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(Modifies, PROCEDURE, "p", VARIABLE, "v", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("a");
			expected.push_back("b");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(Modifies, PROCEDURE, "p", VAR_IDENT, "e", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("a");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(PROCEDURE, "p", NONE);
			q2.insertClause(Modifies, PROCEDURE, "p", VAR_IDENT, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();

			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorNextBefore)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Next, STMT, "s", INTEGER, "4", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getNextBefore(4);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Next, STMT, "s", INTEGER, "2", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expect = pkb.getNextBefore(2);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Next, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorNextAfter)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Next, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getNextAfter(1);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Next, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expect = pkb.getNextAfter(2);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(Next, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}


		TEST_METHOD(PKBEvaluatorNextStarBefore)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(NextT, STMT, "s", INTEGER, "4", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getNextBeforeStar(4);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(NextT, STMT, "s", INTEGER, "2", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expect = pkb.getNextBeforeStar(2);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(NextT, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorNextStarAfter)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(NextT, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getNextAfterStar(1);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(NextT, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expect = pkb.getNextAfterStar(2);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s", NONE);
			q2.insertClause(NextT, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllNext)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s1", NONE);
			q.insertSelectStmt(STMT, "s2", NONE);
			q.insertClause(Next, STMT, "s1", STMT, "s2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllNext();
			for (std::vector<int> r : expect) {
				string a = to_string(r[0]);
				a += " ";
				a += to_string(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllNextStar)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s1", NONE);
			q.insertSelectStmt(STMT, "s2", NONE);
			q.insertClause(NextT, STMT, "s1", STMT, "s2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllNextStar();
			for (std::vector<int> r : expect) {
				string a = to_string(r[0]);
				a += " ";
				a += to_string(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}
		TEST_METHOD(PKBEvaluatorCheckNext)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(Next, INTEGER, "1", INTEGER, "2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(Next, INTEGER, "2", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}
		TEST_METHOD(PKBEvaluatorCheckNextStar)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(NextT, INTEGER, "1", INTEGER, "9", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(NextT, INTEGER, "9", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}

		TEST_METHOD(PKBEvaluatorCallsBefore)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(Calls, PROCEDURE, "p", PROC_IDENT, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getCallsBefore(2);
			for (std::vector<int> r : expect) {
				expected.push_back(pkb.getProcedureName(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(Calls, PROCEDURE, "p", PROC_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorCallsAfterStar)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(CallsT, PROC_IDENT, "a", PROCEDURE, "p",  false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getCallsAfterStar(1);
			for (std::vector<int> r : expect) {
				expected.push_back(pkb.getProcedureName(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(CallsT, PROC_IDENT, "b", PROCEDURE, "p", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorCallsBeforeStar)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(CallsT, PROCEDURE, "p", PROC_IDENT, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getCallsBeforeStar(2);
			for (std::vector<int> r : expect) {
				expected.push_back(pkb.getProcedureName(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(CallsT, PROCEDURE, "p", PROC_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorCallsAfter)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertClause(Calls, PROC_IDENT, "a", PROCEDURE, "p", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getCallsAfter(1);
			for (std::vector<int> r : expect) {
expected.push_back(pkb.getProcedureName(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(PROCEDURE, "p", NONE);
			q1.insertClause(Calls, PROC_IDENT, "b", PROCEDURE, "p", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllCalls)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p1", NONE);
			q.insertSelectStmt(PROCEDURE, "p2", NONE);
			q.insertClause(Calls, PROCEDURE, "p1", PROCEDURE, "p2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllCalls();
			for (std::vector<int> r : expect) {
				string a = pkb.getProcedureName(r[0]);
				a += " ";
				a += pkb.getProcedureName(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllCallsStar)
		{
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p1", NONE);
			q.insertSelectStmt(PROCEDURE, "p2", NONE);
			q.insertClause(CallsT, PROCEDURE, "p1", PROCEDURE, "p2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllCallsStar();
			for (std::vector<int> r : expect) {
				string a = pkb.getProcedureName(r[0]);
				a += " ";
				a += pkb.getProcedureName(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}
		TEST_METHOD(PKBEvaluatorCheckCalls)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(Calls, PROC_IDENT, "a", PROC_IDENT, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(Calls, PROC_IDENT, "b", PROC_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}
		TEST_METHOD(PKBEvaluatorCheckCallsStar)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(CallsT, PROC_IDENT, "a", PROC_IDENT, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(CallsT, PROC_IDENT, "b", PROC_IDENT, "a", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}

		TEST_METHOD(PKBEvaluatorGetCallStatementsCallingProcedure){
			QueryObject q;
			q.insertSelectStmt(CALL, "c", NONE);
			q.insertWithClause(CALL, "c", PROCNAME, PROC_IDENT, "b", NONE, false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();

			result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back({ "4" });
			Assert::AreEqual(true, (result == expected));
		}
		TEST_METHOD(PKBEvaluatorGetProcedureCalledByCallStatement) {
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertWithClause(CALL, "c", PROCNAME, PROCEDURE, "p", PROCNAME, false);
			q.insertWithClause(CALL, "c", STMT_NO, INTEGER, "4", NONE, false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();

			list<string> expected;
			expected.push_back({ "b" });
			Assert::AreEqual(true, (result == expected));

		}
		TEST_METHOD(PKBEvaluatorGetAllCallStatementsCallingProcedure) {
			QueryObject q;
			q.insertSelectStmt(CALL, "c", NONE);
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertWithClause(CALL, "c", PROCNAME, PROCEDURE, "p", PROCNAME, false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();

			list<string> expected;
			expected.push_back({ "4 b" });
			Assert::AreEqual(true, (result == expected));
		}
		TEST_METHOD(PKBEvaluatorGetAllProcedureCalledByCallStatement) {
			QueryObject q;
			q.insertSelectStmt(PROCEDURE, "p", NONE);
			q.insertSelectStmt(CALL, "c", NONE);
			q.insertWithClause(PROCEDURE, "p", PROCNAME, CALL, "c", PROCNAME, false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();

			list<string> expected;
			expected.push_back({ "b 4" });
			Assert::AreEqual(true, (result == expected));
		}
		TEST_METHOD(PKBEvaluatorGetAllCallStatementsWithStmtNo) {
			QueryObject q;
			q.insertSelectStmt(CALL, "c", NONE);
			q.insertWithClause(CALL, "c", STMT_NO, INTEGER, "4", NONE, false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();

			list<string> expected;
			expected.push_back({ "4" });
			Assert::AreEqual(true, (result == expected));


			QueryObject q1;
			q1.insertSelectStmt(CALL, "c", NONE);
			q1.insertWithClause(INTEGER, "4", NONE, CALL, "c", STMT_NO, false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();

			expected.clear();
			expected.push_back({ "4" });
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(PKBEvaluatorAffectsBefore)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Affects, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAffectsBefore(3);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Affects, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAffectsAfterStar)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(AffectsT, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAffectsAfterStar(1);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(AffectsT, INTEGER, "13", STMT, "s", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAffectsBeforeStar)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(AffectsT, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAffectsBeforeStar(3);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(AffectsT, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAffectsAfter)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s", NONE);
			q.insertClause(Affects, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAffectsAfter(1);
			for (std::vector<int> r : expect) {
				expected.push_back(to_string(r[0]));
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;
			q1.insertSelectStmt(STMT, "s", NONE);
			q1.insertClause(Affects, INTEGER, "13", STMT, "s",  false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllAffects)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s1", NONE);
			q.insertSelectStmt(STMT, "s2", NONE);
			q.insertClause(Affects, STMT, "s1", STMT, "s2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllAffects();
			for (std::vector<int> r : expect) {
				string a = to_string(r[0]);
				a += " ";
				a += to_string(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorGetAllAffectsStar)
		{
			QueryObject q;
			q.insertSelectStmt(STMT, "s1", NONE);
			q.insertSelectStmt(STMT, "s2", NONE);
			q.insertClause(AffectsT, STMT, "s1", STMT, "s2", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			std::vector<std::vector<int>> expect = pkb.getAllAffectsStar();
			for (std::vector<int> r : expect) {
				string a = to_string(r[0]);
				a += " ";
				a += to_string(r[1]);
				expected.push_back(a);
			}
			expected.sort();
			Assert::AreEqual(true, (expected == result));
		}
		TEST_METHOD(PKBEvaluatorCheckAffects)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(Affects, INTEGER, "1", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(Affects, INTEGER, "2", INTEGER, "3", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}
		TEST_METHOD(PKBEvaluatorCheckAffectsStar)
		{
			QueryObject q;
			q.insertSelectStmt(BOOLEAN, "", NONE);
			q.insertClause(AffectsT, INTEGER, "1", INTEGER, "3", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "true"));

			QueryObject q1;
			q1.insertSelectStmt(BOOLEAN, "", NONE);
			q1.insertClause(AffectsT, INTEGER, "2", INTEGER, "3", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			Assert::AreEqual(true, (result.front() == "false"));
		}

		TEST_METHOD(PKBEvaluatorPatternAllVarName)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", ALL, "_", EXPR, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("7");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", ALL, "_", EXPR, "f", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorPatternAllConstant)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", ALL, "_", EXPR, "1", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("13");
			expected.push_back("8");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", ALL, "_", EXPR, "17", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorPatternIdentAll)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", VAR_IDENT, "a", ALL, "_", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", VAR_IDENT, "f", ALL, "_", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorPatternIdentVarName)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", VAR_IDENT, "a", EXPR, "b", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("10");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", VAR_IDENT, "f", EXPR, "b", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a", NONE);
			q2.insertPattern(ASSIGN, "a", EXPR, "a", EXPR, "f", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorPatternIdentConstant)
		{
			QueryObject q;
			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", VAR_IDENT, "a", EXPR_EXACT, "|1|", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", VAR_IDENT, "f", EXPR_EXACT, "|1|", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a", NONE);
			q2.insertPattern(ASSIGN, "a", VAR_IDENT, "a", EXPR_EXACT, "|13|", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}


		TEST_METHOD(PKBEvaluatorPatternString)
		{
			QueryObject q;
			q.insertSelectStmt(ASSIGN, "a", NONE);
			q.insertPattern(ASSIGN, "a", VAR_IDENT, "c", EXPR, "|4|b|+|", false);

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("7");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a", NONE);
			q1.insertPattern(ASSIGN, "a", ALL, "_", EXPR, "|x|a|*|", false);

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("7");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a", NONE);
			q2.insertPattern(ASSIGN, "a", VAR_IDENT, "c", EXPR, "|b|4|+|", false);

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(ASSIGN, "a", NONE);
			q3.insertPattern(ASSIGN, "a", VAR_IDENT, "c", EXPR_EXACT, "|4|b|+|x|+|", false);

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			Assert::AreEqual(true, (expected == result));
		}


	};
}