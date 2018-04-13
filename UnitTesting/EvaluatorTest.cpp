#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace EvaluatorTest {

	QueryEvaluator evaluator;
	PKB pkbStub;
	QueryObject queryObjectStub;

	TEST_CLASS(EvaluatorTest) {

		TEST_METHOD(EvaluatorValidQueryObjectSetter) {
			evaluator.setQueryObject(queryObjectStub);
			Assert::AreEqual(true, evaluator.isValidQuery());
		};

		TEST_CLASS_INITIALIZE(setupPKBStub)
		{
			pkbStub.insertToTable(STATEMENT_TABLE, 1, { { 1 },{ 2 },{ 1 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 2, { { 1,2 },{ 3,1 },{ 2,4 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1 },{ 2 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 4, { { 2 },{},{},{ 4 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 5, { { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 6, { { 3,4 },{ 5 },{ 3 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 7, { { 4 },{},{ 3 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 8, { { 5 },{},{ 4 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{ 5 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 10, { { 6 },{ 2 },{ 1 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 11, { { 6,7 },{ 1,2 },{ 1 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 12, { { 7,8 },{ 2 },{ 1 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_TABLE, 13, { { 8 },{},{ 1 },{ 1 } });

			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2,5,9 },{ 1 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3,4 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 3, { { 5 },{ 6 },{ 3 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 4, { { 6 },{ 7 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 5, { { 5 },{ 8 },{ 3 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 6, { { 0 },{ 10,11 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 7, { { 11 },{ 12 },{ 2 } });
			pkbStub.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 13 },{ 2 } });

			pkbStub.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{ 1,2,3,5 },{ 1,2,3,4,5 } });
			pkbStub.insertToTable(PROC_INFO_TABLE, 2, { { 6 },{ 1,2 },{ 1 } });

			pkbStub.insertToTable(USES_TABLE, 1, { { 2,3,5,11 },{ 1,2 } });
			pkbStub.insertToTable(USES_TABLE, 2, { { 1,10,11, 12 },{ 1,2 } });
			pkbStub.insertToTable(USES_TABLE, 3, { { 2 },{ 1 } });
			pkbStub.insertToTable(USES_TABLE, 5, { { 5,6 },{ 1 } });

			pkbStub.insertToTable(MODIFIES_TABLE, 1, { { 1,10,11,12,13 },{ 1,2 } });
			pkbStub.insertToTable(MODIFIES_TABLE, 2, { { 2,3 },{ 1 } });
			pkbStub.insertToTable(MODIFIES_TABLE, 3, { { 5,6,7 },{ 1 } });
			pkbStub.insertToTable(MODIFIES_TABLE, 4, { { 5,8 },{ 1 } });
			pkbStub.insertToTable(MODIFIES_TABLE, 5, { { 9 },{ 1 } });

			pkbStub.insertToTable(CONST_TABLE, 1, { { 8, 9, 13 } });
			pkbStub.insertToTable(CONST_TABLE, 4, { { 7 } });
			pkbStub.insertToTable(CONST_TABLE, 7, { { 4 } });

			pkbStub.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkbStub.insertToTable(NEXT_TABLE, 2, { { 3,5 } });
			pkbStub.insertToTable(NEXT_TABLE, 3, { { 4 } });
			pkbStub.insertToTable(NEXT_TABLE, 4, { { 2 } });
			pkbStub.insertToTable(NEXT_TABLE, 5, { { 6,8 } });
			pkbStub.insertToTable(NEXT_TABLE, 6, { { 7,9 } });
			pkbStub.insertToTable(NEXT_TABLE, 7, { { 6 } });
			pkbStub.insertToTable(NEXT_TABLE, 8, { { 9 } });
			pkbStub.insertToTable(NEXT_TABLE, 10, { { 11 } });
			pkbStub.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkbStub.insertToTable(NEXT_TABLE, 12, { { 11,13 } });
			pkbStub.insertToTable(NEXT_TABLE, 13, { { 12 } });

			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,4 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 5, { { 2 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5,7 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 8, { { 5 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 9, { { 6, 8 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 11, { { 10,12 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 12, { { 11,13 } });
			pkbStub.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });

			pkbStub.insertToTable(CALLS_TABLE, 1, { { 2 },{} });
			pkbStub.insertToTable(CALLS_TABLE, 2, { {},{ 4 } });

			pkbStub.insertToTable(CALLS_INVERSE_TABLE, 2, { { 1 } });
			pkbStub.insertToTable(CALLS_STAR_TABLE, 1, { { 2 } });
			pkbStub.insertToTable(CALLS_STAR_INVERSE_TABLE, 2, { { 1 } });

			pkbStub.insertToTable(CALL_STATEMENT_TABLE, 4, { { 2 } });

			pkbStub.insertToNameTable(PATTERN_TABLE, { "a", "|b|" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "a" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "b", "|a|" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "a" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "e" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "c", "|4|b|+|x|a|*|+" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "d", "|1|" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "e", "|1|" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "a", "|b|" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "a" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "b" });
			pkbStub.insertToNameTable(PATTERN_TABLE, { "a", "|1|" });

			pkbStub.insertToNameTable(PROC_TABLE, { "a" });
			pkbStub.insertToNameTable(PROC_TABLE, { "b" });

			pkbStub.insertToNameTable(VAR_TABLE, { "a" });
			pkbStub.insertToNameTable(VAR_TABLE, { "b" });
			pkbStub.insertToNameTable(VAR_TABLE, { "c" });
			pkbStub.insertToNameTable(VAR_TABLE, { "d" });
			pkbStub.insertToNameTable(VAR_TABLE, { "e" });

			evaluator.setPKB(pkbStub);
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipFollows) 
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Follows, STMT, "s", INTEGER, "2", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;
			expectedResult.push_back("1");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Follows, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipStatementFollowsAfter)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Follows, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Follows, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipProgLineFollowsBefore)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROG_LINE, "n", NONE);
			qo1.insertClause(Follows, PROG_LINE, "n", INTEGER, "2", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROG_LINE, "n", NONE);
			qo2.insertClause(Follows, PROG_LINE, "n", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipProgLineFollowsAfter)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROG_LINE, "n", NONE);
			qo1.insertClause(Follows, INTEGER, "1", PROG_LINE, "n", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROG_LINE, "n", NONE);
			qo2.insertClause(Follows, INTEGER, "9", PROG_LINE, "n", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileFollowsBefore)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Follows, WHILE, "w", INTEGER, "5", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Follows, WHILE, "w", INTEGER, "2", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileFollowsAfter)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Follows, INTEGER, "1", WHILE, "w", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Follows, INTEGER, "5", WHILE, "w", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfFollowsBefore)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Follows, IF, "ifs", INTEGER, "9", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Follows, IF, "ifs", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfFollowsAfter)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Follows, INTEGER, "2", IF, "ifs", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Follows, INTEGER, "9", IF, "ifs", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipFollowsBeforeStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(FollowsT, STMT, "s", INTEGER, "9", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			expectedResult.push_back("2");
			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(FollowsT, STMT, "s", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipFollowsAfterStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROG_LINE, "n", NONE);
			qo1.insertClause(FollowsT, INTEGER, "1", PROG_LINE, "n", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			expectedResult.push_back("5");
			expectedResult.push_back("9");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(FollowsT, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipProgramLineFollowsBeforeStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROG_LINE, "n", NONE);
			qo1.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "9", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			expectedResult.push_back("2");
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROG_LINE, "n", NONE);
			qo2.insertClause(FollowsT, PROG_LINE, "n", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipProgramLineFollowsAfterStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(FollowsT, INTEGER, "1", STMT, "s", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			expectedResult.push_back("5");
			expectedResult.push_back("9");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROG_LINE, "n", NONE);
			qo2.insertClause(FollowsT, INTEGER, "9", STMT, "s", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileFollowsBeforeStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(FollowsT, WHILE, "w", INTEGER, "9", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(FollowsT, WHILE, "w", INTEGER, "2", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileFollowsAfterStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(FollowsT, INTEGER, "1", WHILE, "w", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(FollowsT, INTEGER, "9", WHILE, "w", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfFollowsBeforeStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(FollowsT, IF, "ifs", INTEGER, "9", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(FollowsT, IF, "ifs", INTEGER, "1", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfFollowsAfterStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(FollowsT, INTEGER, "1", IF, "ifs", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(FollowsT, INTEGER, "9", IF, "ifs", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipChildren)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Parent, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("3");
			expectedResult.push_back("4");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Parent, INTEGER, "5", STMT, "s", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("6");
			expectedResult.push_back("8");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(Parent, INTEGER, "11", STMT, "s", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("12");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo4;

			qo4.insertSelectStmt(STMT, "s", NONE);
			qo4.insertClause(Parent, INTEGER, "12", STMT, "s", false);

			evaluator.setQueryObject(qo4);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult .push_back("13");

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParent)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Parent, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Parent, STMT, "s", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(Parent, STMT, "s", INTEGER, "9", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParentIf)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Parent, IF, "ifs", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Parent, IF, "ifs", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(IF, "ifs", NONE);
			qo3.insertClause(Parent, IF, "ifs", INTEGER, "8", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo4;

			qo4.insertSelectStmt(IF, "ifs", NONE);
			qo4.insertClause(Parent, IF, "ifs", INTEGER, "7", false);

			evaluator.setQueryObject(qo4);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParentWhile)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Parent, WHILE, "w", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Parent, WHILE, "w", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(WHILE, "w", NONE);
			qo3.insertClause(Parent, WHILE, "w", INTEGER, "1", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipChildrenStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(ParentT, INTEGER, "2", STMT, "s", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("3");
			expectedResult.push_back("4");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(ParentT, INTEGER, "5", STMT, "s", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("6");
			expectedResult.push_back("7");
			expectedResult.push_back("8");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(ParentT, INTEGER, "11", STMT, "s", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("12");
			expectedResult.push_back("13");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo4;

			qo4.insertSelectStmt(STMT, "s", NONE);
			qo4.insertClause(ParentT, INTEGER, "12", STMT, "s", false);

			evaluator.setQueryObject(qo4);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("13");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo5;

			qo5.insertSelectStmt(STMT, "s", NONE);
			qo5.insertClause(ParentT, INTEGER, "13", STMT, "s", false);

			evaluator.setQueryObject(qo5);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParentStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Parent, STMT, "s", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Parent, STMT, "s", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(Parent, STMT, "s", INTEGER, "10", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParentIfStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Parent, IF, "ifs", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Parent, IF, "ifs", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(IF, "ifs", NONE);
			qo3.insertClause(Parent, IF, "ifs", INTEGER, "7", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("6");

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipParentWhileStar)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Parent, WHILE, "w", INTEGER, "3", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("2");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Parent, WHILE, "w", INTEGER, "6", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(WHILE, "w", NONE);
			qo3.insertClause(Parent, WHILE, "w", INTEGER, "8", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipStatementNumberUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(VARIABLE, "v", NONE);
			qo1.insertClause(Uses, INTEGER, "1", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("b");

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipStatementUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Uses, STMT, "s", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("11");
			expectedResult.push_back("12");
			expectedResult.push_back("2");
			expectedResult.push_back("3");
			expectedResult.push_back("5");
			expectedResult.push_back("6");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Uses, STMT, "s", VAR_IDENT, "b", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("11");
			expectedResult.push_back("12");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(Uses, STMT, "s", VAR_IDENT, "f", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Uses, IF, "ifs", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Uses, IF, "ifs", VAR_IDENT, "a", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(IF, "ifs", NONE);
			qo3.insertClause(Uses, IF, "ifs", VAR_IDENT, "b", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Uses, WHILE, "w", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("11");
			expectedResult.push_back("12");
			expectedResult.push_back("2");
			expectedResult.push_back("6");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Uses, WHILE, "w", VAR_IDENT, "c", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("2");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(WHILE, "w", NONE);
			qo3.insertClause(Uses, WHILE, "w", VAR_IDENT, "d", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipAssignUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(ASSIGN, "a", NONE);
			qo1.insertClause(Uses, ASSIGN, "a", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("3");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(ASSIGN, "a", NONE);
			qo2.insertClause(Uses, ASSIGN, "a", VAR_IDENT, "a", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("3");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(ASSIGN, "a", NONE);
			qo3.insertClause(Uses, ASSIGN, "a", VAR_IDENT, "e", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}


		TEST_METHOD(EvaluatorEvaluateRelationshipProcedureUsesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROCEDURE, "p", NONE);
			qo1.insertClause(Uses, PROCEDURE, "p", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("a");
			expectedResult.push_back("b");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROCEDURE, "p", NONE);
			qo2.insertClause(Uses, PROCEDURE, "p", VAR_IDENT, "e", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("a");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(PROCEDURE, "p", NONE);
			qo3.insertClause(Uses, PROCEDURE, "p", VAR_IDENT, "f", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipStatementNumberModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(VARIABLE, "v", NONE);
			qo1.insertClause(Modifies, INTEGER, "1", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("a");

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipStatementModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(STMT, "s", NONE);
			qo1.insertClause(Modifies, STMT, "s", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;
			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("11");
			expectedResult.push_back("12");
			expectedResult.push_back("13");
			expectedResult.push_back("2");
			expectedResult.push_back("3");
			expectedResult.push_back("5");
			expectedResult.push_back("6");
			expectedResult.push_back("7");
			expectedResult.push_back("8");
			expectedResult.push_back("9");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(STMT, "s", NONE);
			qo2.insertClause(Modifies, STMT, "s", VAR_IDENT, "b", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("2");
			expectedResult.push_back("3");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(STMT, "s", NONE);
			qo3.insertClause(Modifies, STMT, "s", VAR_IDENT, "f", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipIfModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(IF, "ifs", NONE);
			qo1.insertClause(Modifies, IF, "ifs", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(IF, "ifs", NONE);
			qo2.insertClause(Modifies, IF, "ifs", VAR_IDENT, "c", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("5");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(IF, "ifs", NONE);
			qo3.insertClause(Modifies, IF, "ifs", VAR_IDENT, "b", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipWhileModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(WHILE, "w", NONE);
			qo1.insertClause(Modifies, WHILE, "w", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("11");
			expectedResult.push_back("12");
			expectedResult.push_back("2");
			expectedResult.push_back("6");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(WHILE, "w", NONE);
			qo2.insertClause(Modifies, WHILE, "w", VAR_IDENT, "b", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("2");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(WHILE, "w", NONE);
			qo3.insertClause(Modifies, WHILE, "w", VAR_IDENT, "d", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipAssignModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(ASSIGN, "a", NONE);
			qo1.insertClause(Modifies, ASSIGN, "a", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("13");
			expectedResult.push_back("3");
			expectedResult.push_back("7");
			expectedResult.push_back("8");
			expectedResult.push_back("9");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(ASSIGN, "a", NONE);
			qo2.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "a", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			expectedResult.push_back("1");
			expectedResult.push_back("10");
			expectedResult.push_back("13");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(ASSIGN, "a", NONE);
			qo3.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "f", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

		TEST_METHOD(EvaluatorEvaluateRelationshipProcedureModifiesVariable)
		{
			QueryObject qo1;

			qo1.insertSelectStmt(PROCEDURE, "p", NONE);
			qo1.insertClause(Modifies, PROCEDURE, "p", VARIABLE, "v", false);

			evaluator.setQueryObject(qo1);

			list<string> actualResult = evaluator.evaluateQuery();
			list<string> expectedResult;

			expectedResult.push_back("a");
			expectedResult.push_back("b");
			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo2;

			qo2.insertSelectStmt(PROCEDURE, "p", NONE);
			qo2.insertClause(Modifies, PROCEDURE, "p", VAR_IDENT, "e", false);

			evaluator.setQueryObject(qo2);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();
			expectedResult.push_back("a");

			Assert::AreEqual(true, (expectedResult == actualResult));

			QueryObject qo3;

			qo3.insertSelectStmt(PROCEDURE, "p", NONE);
			qo3.insertClause(Modifies, PROCEDURE, "p", VAR_IDENT, "f", false);

			evaluator.setQueryObject(qo3);

			actualResult = evaluator.evaluateQuery();
			expectedResult.clear();

			Assert::AreEqual(true, (expectedResult == actualResult));
		}

	};
}