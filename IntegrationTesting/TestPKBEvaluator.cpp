#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/Evaluator.h";
#include "../SPA/QueryObject.h";
#include "../SPA/Pattern.h";
#include "../SPA/Clause.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBEvaluatorIntegrationTesting
{
	PKB pkb;
	Evaluator evaluator;
	list<string> _variableID;

	TEST_CLASS(PKBAPIIntegration)
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
			unordered_map<int, std::vector<std::vector<int>>> table0;
			table0.insert({ 1, { {1}, {2}, {1}, {1} } });
			table0.insert({ 2,{ { 1,2 },{ 3,1 },{ 2,4 },{ 2 } } });
			table0.insert({ 3,{ { 2 },{ 1 },{2},{ 1 } } });
			table0.insert({ 4,{ { 2 },{  },{ 4 },{ 1 } } });
			table0.insert({ 5,{ { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } } });
			table0.insert({ 6,{ { 3,4 },{ 5 },{ 3 },{ 2 } } });
			table0.insert({ 7,{ { 4 },{  },{ 3 },{ 1 } } });
			table0.insert({ 8,{ { 5 }, {},{ 4 },{ 1 } } });
			table0.insert({ 9,{ { 1 },{},{ 5 },{ 1 } } });
			table0.insert({ 10,{ { 6 },{2},{ 1},{ 1 } } });
			table0.insert({ 11,{ { 6,7 },{ 1,2 },{ 1 },{ 2 } } });
			table0.insert({ 12,{ { 7,8 },{ 2 },{ 1 },{ 2 } } });
			table0.insert({ 13,{ { 8 },{ },{1 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table1;
			table1.insert({ 1, {{0},{1,2,5,9}, {1}} });
			table1.insert({ 2,{ { 2 },{ 3,4 },{ 2 } } });
			table1.insert({ 3,{ { 5 },{ 6 },{ 3 } } });
			table1.insert({ 4,{ { 6 },{ 7 },{ 2 } } });
			table1.insert({ 5,{ { 5 },{ 8 },{ 3 } } });
			table1.insert({ 6,{ { 0 },{ 10,11 },{ 2 } } });
			table1.insert({ 7,{ { 11 },{ 12 },{ 2 } } });
			table1.insert({ 8,{ { 12 },{ 13 },{ 2 } } });

			unordered_map<int, std::vector<std::vector<int>>> table2;
			table2.insert({ 1, {{1},{1,2,3,5},{1,2,3,4,5}} });
			table2.insert({ 2,{ { 6 },{ 1,2 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table3;
			table3.insert({ 1, {{2,3,5,11}, {1,2}} });
			table3.insert({ 2,{ {1,10,11},{ 1,2 } } });
			table3.insert({ 3,{ { 2 },{ 1 } } });
			table3.insert({ 5,{ {5,6},{1} } });

			unordered_map<int, std::vector<std::vector<int>>> table4;
			table4.insert({ 1,{ {1,10,11,12,13},{1,2} } });
			table4.insert({ 2,{ { 2,3 },{ 1 } } });
			table4.insert({ 3,{ { 5,6,7 },{ 1 } } });
			table4.insert({ 4,{ { 5,8 },{ 1 } } });
			table4.insert({ 5,{ { 9 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table6;
			table6.insert({ 1, {{8, 9, 13}} });
			table6.insert({ 4,{ { 7 } } });
			table6.insert({ 7,{ { 4 } } });

			unordered_map<int, string> table7;
			table7.insert({1, "a"});
			table7.insert({ 2, "b" });

			unordered_map<int, string> table8;
			table8.insert({ 1, "a" });
			table8.insert({ 2, "b" });
			table8.insert({ 3, "c" });
			table8.insert({ 4, "d" });
			table8.insert({ 5, "e" });

			pkb.tables[0] = table0;
			pkb.tables[1] = table1;
			pkb.tables[2] = table2;
			pkb.tables[3] = table3;
			pkb.tables[4] = table4;
			pkb.tables[6] = table6;
			pkb.nameTables[0] = table7;
			pkb.nameTables[1] = table8;

			evaluator.setPKB(pkb);
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(Follows, STMT, "s", INTEGER, "2");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(Follows, STMT, "s", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsAfter)
		{
			QueryObject q;
			
			q.insertSelectStmt(STMT, "s");
			q.insertClause(Follows, INTEGER, "1", STMT, "s");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(Follows, INTEGER, "9", STMT, "s");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n");
			q.insertClause(Follows, PROG_LINE, "n", INTEGER, "2");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n");
			q1.insertClause(Follows, PROG_LINE, "n", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n");
			q.insertClause(Follows, INTEGER, "1", PROG_LINE, "n");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n");
			q1.insertClause(Follows, INTEGER, "9", PROG_LINE, "n");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(Follows, WHILE, "w", INTEGER, "5");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(Follows, WHILE, "w", INTEGER, "2");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(Follows, INTEGER, "1", WHILE, "w");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(Follows, INTEGER, "5", WHILE, "w");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsBefore)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(Follows, IF, "ifs", INTEGER, "9");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(Follows, IF, "ifs", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsAfter)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(Follows, INTEGER, "2", IF, "ifs");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(Follows, INTEGER, "9", IF, "ifs");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(FollowsT, STMT, "s", INTEGER, "9");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(FollowsT, STMT, "s", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n");
			q.insertClause(FollowsT, INTEGER, "1", PROG_LINE, "n");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("5");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(PROG_LINE, "n");
			q.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "9");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n");
			q1.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorProgLineFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(FollowsT, INTEGER, "1", STMT, "s");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("5");
			expected.push_back("9");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(PROG_LINE, "n");
			q1.insertClause(FollowsT, INTEGER, "9", STMT, "s");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(FollowsT, WHILE, "w", INTEGER, "9");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(FollowsT, WHILE, "w", INTEGER, "2");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(FollowsT, INTEGER, "1", WHILE, "w");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(FollowsT, INTEGER, "9", WHILE, "w");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsBeforeStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(FollowsT, IF, "ifs", INTEGER, "9");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(FollowsT, IF, "ifs", INTEGER, "1");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfFollowsAfterStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(FollowsT, INTEGER, "1", IF, "ifs");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(FollowsT, INTEGER, "9", IF, "ifs");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorChildren)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(Parent, INTEGER, "2", STMT, "s");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("3");
			expected.push_back("4");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(Parent, INTEGER, "5", STMT, "s");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("6");
			expected.push_back("8");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(Parent, INTEGER, "11", STMT, "s");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(STMT, "s");
			q3.insertClause(Parent, INTEGER, "12", STMT, "s");

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParent)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(Parent, STMT, "s", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(Parent, STMT, "s", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(Parent, STMT, "s", INTEGER, "9");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentIf)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(Parent, IF, "ifs", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(Parent, IF, "ifs", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs");
			q2.insertClause(Parent, IF, "ifs", INTEGER, "8");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(IF, "ifs");
			q3.insertClause(Parent, IF, "ifs", INTEGER, "7");

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentWhile)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(Parent, WHILE, "w", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(Parent, WHILE, "w", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w");
			q2.insertClause(Parent, WHILE, "w", INTEGER, "1");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorChildrenStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(ParentT, INTEGER, "2", STMT, "s");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("3");
			expected.push_back("4");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(ParentT, INTEGER, "5", STMT, "s");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("6");
			expected.push_back("7");
			expected.push_back("8");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(ParentT, INTEGER, "11", STMT, "s");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("12");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q3;

			q3.insertSelectStmt(STMT, "s");
			q3.insertClause(ParentT, INTEGER, "12", STMT, "s");

			evaluator.setQueryObject(q3);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q4;

			q4.insertSelectStmt(STMT, "s");
			q4.insertClause(ParentT, INTEGER, "13", STMT, "s");

			evaluator.setQueryObject(q4);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentStar)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(Parent, STMT, "s", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(Parent, STMT, "s", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(Parent, STMT, "s", INTEGER, "10");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentIfStar)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(Parent, IF, "ifs", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(Parent, IF, "ifs", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs");
			q2.insertClause(Parent, IF, "ifs", INTEGER, "7");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorParentWhileStar)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(Parent, WHILE, "w", INTEGER, "3");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(Parent, WHILE, "w", INTEGER, "6");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w");
			q2.insertClause(Parent, WHILE, "w", INTEGER, "8");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementNumberUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(VARIABLE, "v");
			q.insertClause(UsesS, INTEGER, "1", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("b");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(UsesS, STMT, "s", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("3");
			expected.push_back("5");
			expected.push_back("6");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(UsesS, STMT, "s", IDENT, "b");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(UsesS, STMT, "s", IDENT, "f");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(UsesS, IF, "ifs", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(UsesS, IF, "ifs", IDENT, "a");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs");
			q2.insertClause(UsesS, IF, "ifs", IDENT, "b");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(UsesS, WHILE, "w", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("6");
			expected.push_back("11");
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(UsesS, WHILE, "w", IDENT, "c");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w");
			q2.insertClause(UsesS, WHILE, "w", IDENT, "d");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAssignUsesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a");
			q.insertClause(UsesS, ASSIGN, "a", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("3");
			expected.push_back("10");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a");
			q1.insertClause(UsesS, ASSIGN, "a", IDENT, "a");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("3");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a");
			q2.insertClause(UsesS, ASSIGN, "a", IDENT, "e");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}


		TEST_METHOD(PKBEvaluatorStatementNumberModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(VARIABLE, "v");
			q.insertClause(ModifiesS, INTEGER, "1", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("a");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorStatementModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(STMT, "s");
			q.insertClause(ModifiesS, STMT, "s", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("5");
			expected.push_back("6");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("9");
			expected.push_back("10");
			expected.push_back("11");
			expected.push_back("12");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(STMT, "s");
			q1.insertClause(ModifiesS, STMT, "s", IDENT, "b");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			expected.push_back("3");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(STMT, "s");
			q2.insertClause(ModifiesS, STMT, "s", IDENT, "f");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorIfModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(IF, "ifs");
			q.insertClause(ModifiesS, IF, "ifs", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(IF, "ifs");
			q1.insertClause(ModifiesS, IF, "ifs", IDENT, "c");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("5");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(IF, "ifs");
			q2.insertClause(ModifiesS, IF, "ifs", IDENT, "b");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorWhileModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(WHILE, "w");
			q.insertClause(ModifiesS, WHILE, "w", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			expected.push_back("6");
			expected.push_back("11");
			expected.push_back("12");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(WHILE, "w");
			q1.insertClause(ModifiesS, WHILE, "w", IDENT, "b");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(WHILE, "w");
			q2.insertClause(ModifiesS, WHILE, "w", IDENT, "d");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

		TEST_METHOD(PKBEvaluatorAssignModifiesVariable)
		{
			QueryObject q;

			q.insertSelectStmt(ASSIGN, "a");
			q.insertClause(ModifiesS, ASSIGN, "a", VARIABLE, "v");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("3");
			expected.push_back("4");
			expected.push_back("7");
			expected.push_back("8");
			expected.push_back("9");
			expected.push_back("10");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q1;

			q1.insertSelectStmt(ASSIGN, "a");
			q1.insertClause(ModifiesS, ASSIGN, "a", IDENT, "a");

			evaluator.setQueryObject(q1);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("1");
			expected.push_back("10");
			expected.push_back("13");
			Assert::AreEqual(true, (expected == result));

			QueryObject q2;

			q2.insertSelectStmt(ASSIGN, "a");
			q2.insertClause(ModifiesS, ASSIGN, "a", IDENT, "f");

			evaluator.setQueryObject(q2);

			result = evaluator.evaluateQuery();
			expected.clear();
			expected.push_back("None");
			Assert::AreEqual(true, (expected == result));
		}

	};
}