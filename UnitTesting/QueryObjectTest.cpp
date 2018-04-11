#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryObject.h"
#include "../SPA/Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace QueryObjectTest {
	TEST_CLASS(QueryObjectTest) {
	public:
		TEST_METHOD(QueryObjectInsertSelectStmt) {
			QueryObject queryObject;
			queryObject.insertSelectStmt(ASSIGN, "a", NONE);

			Assert::AreEqual(1, static_cast<int>(queryObject.getSelectStatements().size()));

			Param testParam;
			testParam.type = ASSIGN;
			testParam.value = "a";
			testParam.attribute = NONE;

			Param fromQueryObject = queryObject.getSelectStatements().at(0);
			Assert::AreEqual(true, Utils::compareParam(fromQueryObject, testParam));
		}

		TEST_METHOD(QueryObjectInsertClause) {
			QueryObject queryObject;
			queryObject.insertClause(Parent, INTEGER, "1", ASSIGN, "a", false);

			Assert::AreEqual(1, static_cast<int>(queryObject.getClauses().size()));

			Param leftArg;
			leftArg.type = INTEGER;
			leftArg.value = "1";
			leftArg.attribute = NONE;

			Param rightArg;
			rightArg.type = ASSIGN;
			rightArg.value = "a";
			rightArg.attribute = NONE;

			Clause testClause(Parent, leftArg, rightArg, false);

			Clause fromQueryObject = queryObject.getClauses().at(0);
			Assert::AreEqual(true, Utils::compareClause(fromQueryObject, testClause));
		}

		TEST_METHOD(QueryObjectInsertPattern) {
			QueryObject queryObject;
			queryObject.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "_\"a\"_", false);

			Assert::AreEqual(1, static_cast<int>(queryObject.getPatterns().size()));

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

			Pattern fromQueryObject = queryObject.getPatterns().at(0);

			Assert::AreEqual(true, Utils::comparePattern(fromQueryObject, testPattern));
		}

		TEST_METHOD(QueryObjectInsertWithClause) {
			QueryObject queryObject;
			queryObject.insertWithClause(STMT, "s", STMT_NO, PROG_LINE, "n", NONE, false);

			Assert::AreEqual(1, static_cast<int>(queryObject.getWithClauses().size()));

			Param leftArg;
			leftArg.type = STMT;
			leftArg.value = "s";
			leftArg.attribute = STMT_NO;

			Param rightArg;
			rightArg.type = PROG_LINE;
			rightArg.value = "n";
			rightArg.attribute = NONE;

			Clause testWithClause(With, leftArg, rightArg, false);

			Clause fromQueryObject = queryObject.getWithClauses().at(0);
			Assert::AreEqual(true, Utils::compareClause(fromQueryObject, testWithClause));
		}
	};
}