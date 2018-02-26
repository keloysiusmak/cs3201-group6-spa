#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Evaluator.h"
#include "./PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {

	Evaluator evaluator;
	PKB pkbStub;
	QueryObject queryObjectStub;

	TEST_CLASS(EvaluatorTest) {

		TEST_METHOD(ValidQueryObjectSetterTest) {
			evaluator.setQueryObject(queryObjectStub);
			Assert::AreEqual(true, evaluator.isValidQuery());
		}

		TEST_METHOD(InvalidQueryObjectSetterTest) {
			evaluator.setInvalidQuery("Invalid Query");
			Assert::AreEqual(false, evaluator.isValidQuery());
		}

		TEST_METHOD(SelectParamInClauseTest) {
			Param select, clauseLHS, clauseRHS, patternEnt, patternLHS, patternRHS;
			Clause clause;
			Pattern pattern;
			QueryObject queryObj;

			/* stmt s; Select s such that Follows(1, s) */
			clauseLHS = createParam(INTEGER, "1");
			clauseRHS = createParam(STMT, "s");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			select = createParam(STMT, "s");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			///* while w; Select w such that Follows(w, 2) */
			clauseLHS = createParam(WHILE, "w");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			select = createParam(WHILE, "w");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* stmt s; while w; Select w such that Follows(w, s) */
			clauseLHS = createParam(WHILE, "w");
			clauseRHS = createParam(STMT, "s");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			select = createParam(WHILE, "w");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* variable v; Select w such that Follows(1, 2)pattern(v, 1) */
			clauseLHS = createParam(INTEGER, "1");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			patternEnt = createParam(ASSIGN, "a");
			patternLHS = createParam(VARIABLE, "v");
			patternRHS = createParam(CONSTANT, "1");
			pattern = createPattern(patternEnt, patternLHS, patternRHS);
			select = createParam(VARIABLE, "v");
			queryObj = createQueryObject(select, clause, pattern);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* stmt s; while w; Select w such that Follows(1, 2)pattern("c", v) */
			clauseLHS = createParam(INTEGER, "1");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			patternEnt = createParam(ASSIGN, "a");
			patternLHS = createParam(VAR_NAME, "c");
			patternRHS = createParam(VARIABLE, "v");
			pattern = createPattern(patternEnt, patternLHS, patternRHS);
			select = createParam(VARIABLE, "v");
			queryObj = createQueryObject(select, clause, pattern);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
		}

		TEST_METHOD(SelectParamNotInClauseTest) {
			/* stmt s; while w; Select w such that Follows(3, s) */
			Param lhs = createParam(INTEGER, "3");
			Param rhs = createParam(STMT, "s");
			Clause clause = createClause(Follows, lhs, rhs);
			Param select = createParam(WHILE, "w");
			QueryObject queryObj = createQueryObject(select, clause);
			Assert::AreEqual(false, evaluator.selectParamInClauses(queryObj));
		}



		TEST_METHOD(QueryHasClauseTest) {
			
		}

		/* Object creation helpers*/
		Param createParam(ParamType type, string value) {
			Param param;
			param.type = type;
			param.value = value;
			return param;
		};

		Clause createClause(RelRef rel, Param lhs, Param rhs) {
			return Clause(rel, lhs, rhs);
		};

		Pattern createPattern(Param ent, Param lhs, Param rhs) {
			return Pattern(ent, lhs, rhs);
		}

		/* Overloaded QueryObject constructor methods */

		/* Only select statement */
		QueryObject createQueryObject(Param select) {
			QueryObject queryObj;
			queryObj.insertSelectStmt(select.type, select.value);
			return queryObj;
		};

		/* Clause present */
		QueryObject createQueryObject(Param select, Clause clause) {
			QueryObject queryObj;
			queryObj.insertSelectStmt(select.type, select.value);
			queryObj.insertClause(clause.getRelRef(), clause.getFirstParam().type,
				clause.getFirstParam().value, clause.getSecondParam().type, clause.getSecondParam().value);
			return queryObj;
		};

		/* Clause and Pattern present */
		QueryObject createQueryObject(Param select, Clause clause, Pattern pattern) {
			QueryObject queryObj;
			queryObj.insertSelectStmt(select.type, select.value);
			queryObj.insertClause(clause.getRelRef(), clause.getFirstParam().type,
				clause.getFirstParam().value, clause.getSecondParam().type, clause.getSecondParam().value);
			queryObj.insertPattern(pattern.getEntity().type, pattern.getEntity().value,
				pattern.getLeftParam().type, pattern.getLeftParam().value,
				pattern.getRightParam().type, pattern.getRightParam().value);
			return queryObj;
		};
	};
}