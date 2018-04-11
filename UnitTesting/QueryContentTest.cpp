#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryContent.h";
#include "../SPA/Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace QueryContentTest {
	TEST_CLASS(QueryContentTest) {
public:
	TEST_METHOD(QueryContentInsertSelectStmt) {
		QueryContent queryContent;
		queryContent.insertSelect(ASSIGN, "a", NONE);

		Assert::AreEqual(1, static_cast<int>(queryContent.getSelect().size()));

		Param testParam;
		testParam.type = ASSIGN;
		testParam.value = "a";
		testParam.attribute = NONE;

		Param fromQueryObject = queryContent.getSelect().at(0);
		Assert::AreEqual(true, Utils::compareParam(fromQueryObject, testParam));
	}

	TEST_METHOD(QueryContentInsertClause) {
		QueryContent queryContent;
		queryContent.insertClause(Parent, INTEGER, "1", ASSIGN, "a", false);

		Assert::AreEqual(1, static_cast<int>(queryContent.getClauses().size()));

		Param leftArg;
		leftArg.type = INTEGER;
		leftArg.value = "1";
		leftArg.attribute = NONE;

		Param rightArg;
		rightArg.type = ASSIGN;
		rightArg.value = "a";
		rightArg.attribute = NONE;

		Clause testClause(Parent, leftArg, rightArg, false);

		Clause fromQueryContent = queryContent.getClauses().at(0).getClause();
		Assert::AreEqual(true, Utils::compareClause(fromQueryContent, testClause));
	}

	TEST_METHOD(QueryContentInsertPattern) {
		QueryContent queryContent;
		queryContent.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "_\"a\"_", false);

		Assert::AreEqual(1, static_cast<int>(queryContent.getPattern().size()));

		Param pattern;
		pattern.type = ASSIGN;
		pattern.value = "a";
		pattern.attribute = NONE;

		Param leftArg;
		leftArg.type = VARIABLE;
		leftArg.value = "v";
		leftArg.attribute = NONE;

		Param rightArg;
		rightArg.type = EXPR;
		rightArg.value = "_\"a\"_";
		rightArg.attribute = NONE;

		Pattern testPattern(pattern, leftArg, rightArg, false);

		Pattern fromQueryContent = queryContent.getPattern().at(0).getPattern();

		Assert::AreEqual(true, Utils::comparePattern(fromQueryContent, testPattern));
	}

	TEST_METHOD(QueryContentInsertWithClause) {
		QueryContent queryContent;
		queryContent.insertWithClause(STMT, "s", STMT_NO, PROG_LINE, "n", NONE, false);

		Assert::AreEqual(1, static_cast<int>(queryContent.getWithClauses().size()));

		Param leftArg;
		leftArg.type = STMT;
		leftArg.value = "s";
		leftArg.attribute = STMT_NO;

		Param rightArg;
		rightArg.type = PROG_LINE;
		rightArg.value = "n";
		rightArg.attribute = NONE;

		Clause testWithClause(With, leftArg, rightArg, false);

		Clause fromQueryContent = queryContent.getWithClauses().at(0).getWithClause();
		Assert::AreEqual(true, Utils::compareClause(fromQueryContent, testWithClause));
	}

	TEST_METHOD(QueryContentInsertOperator) {
		QueryContent queryContent;
		queryContent.insertOperator(CLAUSE, AND);

		Assert::AreEqual(1, static_cast<int>(queryContent.getClauses().size()));
	}

	TEST_METHOD(QueryContentTestNegation) {
		QueryContent queryContent;

		queryContent.insertOperator(CLAUSE, AND);
		queryContent.insertClause(Parent, INTEGER, "1", ASSIGN, "a", false);

		Assert::AreEqual(2, static_cast<int>(queryContent.getClauses().size()));

		queryContent.setNegation(CLAUSE, 0);
		queryContent.setNegation(CLAUSE, 1);

		Param leftArg;
		leftArg.type = INTEGER;
		leftArg.value = "1";
		leftArg.attribute = NONE;

		Param rightArg;
		rightArg.type = ASSIGN;
		rightArg.value = "a";
		rightArg.attribute = NONE;

		Clause testClause(Parent, leftArg, rightArg, true);

		Clause fromQueryContent = queryContent.getClauses().at(1).getClause();
		Assert::AreEqual(static_cast<int>(OR), static_cast<int>(queryContent.getClauses().at(0).getOperators()));
		Assert::AreEqual(true, Utils::compareClause(fromQueryContent, testClause));
	}
	};
}