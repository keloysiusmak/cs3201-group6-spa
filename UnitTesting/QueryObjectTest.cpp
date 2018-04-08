#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace QueryObjectTest {
	TEST_CLASS(QueryObjectTest) {
	public:
		TEST_METHOD(QueryObjectInsertSelectStmt) {
			QueryObject queryObject;
			queryObject.insertSelectStmt(ASSIGN, "a", NONE);

			Param testParam;
			testParam.type = ASSIGN;
			testParam.value = "a";

			//Assert::AreEqual(static_cast<int>(testParam.type), static_cast<int>(queryObject.getSelectStatements().type));
			//Assert::AreEqual(testParam.value, queryObject.getSelectStatements().value);
		}

		TEST_METHOD(QueryObjectInsertClause) {
			QueryObject queryObject;
			queryObject.insertClause(Parent, INTEGER, "1", ASSIGN, "a", false);

			Assert::AreEqual(1, static_cast<int>(queryObject.getClauses().size()));

			Param leftArg;
			leftArg.type = INTEGER;
			leftArg.value = "1";

			Param rightArg;
			rightArg.type = ASSIGN;
			rightArg.value = "a";

			Clause testClause(Parent, leftArg, rightArg, false);

			Clause fromQueryObject = queryObject.getClauses().at(0);

			Assert::AreEqual(static_cast<int>(testClause.getRelRef()), 
				static_cast<int>(fromQueryObject.getRelRef()));
			Assert::AreEqual(static_cast<int>(testClause.getLeftParam().type), 
						static_cast<int>(fromQueryObject.getLeftParam().type));
			Assert::AreEqual(testClause.getLeftParam().value, fromQueryObject.getLeftParam().value);
			Assert::AreEqual(static_cast<int>(testClause.getRightParam().type),
				static_cast<int>(fromQueryObject.getRightParam().type));
			Assert::AreEqual(testClause.getRightParam().value, fromQueryObject.getRightParam().value);

		}

		TEST_METHOD(QueryObjectInsertPattern) {
			QueryObject queryObject;
			queryObject.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "_\"a\"_", false);

			Assert::AreEqual(1, static_cast<int>(queryObject.getPatterns().size()));

			Param pattern;
			pattern.type = ASSIGN;
			pattern.value = "a";

			Param leftArg;
			leftArg.type = VARIABLE;
			leftArg.value = "v";

			Param rightArg;
			rightArg.type = EXPR;
			rightArg.value = "_\"a\"_";

			Pattern testPattern(pattern, leftArg, rightArg, false);

			Pattern fromQueryObject = queryObject.getPatterns().at(0);

			Assert::AreEqual(static_cast<int>(testPattern.getEntity().type),
				static_cast<int>(fromQueryObject.getEntity().type));
			Assert::AreEqual(testPattern.getEntity().value,
				fromQueryObject.getEntity().value);
			Assert::AreEqual(static_cast<int>(testPattern.getLeftParam().type),
				static_cast<int>(fromQueryObject.getLeftParam().type));
			Assert::AreEqual(testPattern.getLeftParam().value, fromQueryObject.getLeftParam().value);
			Assert::AreEqual(static_cast<int>(testPattern.getRightParam().type),
				static_cast<int>(fromQueryObject.getRightParam().type));
			Assert::AreEqual(testPattern.getRightParam().value, fromQueryObject.getRightParam().value);

		}
	};
}