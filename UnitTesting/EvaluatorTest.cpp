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

			/* while w; Select w such that Follows(w, 2) */
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

			/* if ifs; Select ifs such that Follows(ifs, 2) */
			clauseLHS = createParam(IF, "ifs");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Follows, clauseLHS, clauseRHS);
			select = createParam(IF, "ifs");
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

			/* stmt s; Select s such that Parent(3, s) */
			clauseLHS = createParam(INTEGER, "3");
			clauseRHS = createParam(STMT, "s");
			clause = createClause(Parent, clauseLHS, clauseRHS);
			select = createParam(STMT, "s");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* while w; Select w such that Parent(w, 4) */
			clauseLHS = createParam(WHILE, "w");
			clauseRHS = createParam(INTEGER, "4");
			clause = createClause(Parent, clauseLHS, clauseRHS);
			select = createParam(WHILE, "w");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* stmt s; while w; Select w such that Parent(w, s) */
			clauseLHS = createParam(WHILE, "w");
			clauseRHS = createParam(STMT, "s");
			clause = createClause(Parent, clauseLHS, clauseRHS);
			select = createParam(WHILE, "w");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* variable v; Select v such that Parent(1, 2) pattern a (v, 1) */
			clauseLHS = createParam(INTEGER, "1");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Parent, clauseLHS, clauseRHS);
			patternEnt = createParam(ASSIGN, "a");
			patternLHS = createParam(VARIABLE, "v");
			patternRHS = createParam(CONSTANT, "1");
			pattern = createPattern(patternEnt, patternLHS, patternRHS);
			select = createParam(VARIABLE, "v");
			queryObj = createQueryObject(select, clause, pattern);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));

			/* variable v; Select w such that Parent(1, 2) pattern a ("c", v) */
			clauseLHS = createParam(INTEGER, "1");
			clauseRHS = createParam(INTEGER, "2");
			clause = createClause(Parent, clauseLHS, clauseRHS);
			patternEnt = createParam(ASSIGN, "a");
			patternLHS = createParam(VAR_NAME, "c");
			patternRHS = createParam(VARIABLE, "v");
			pattern = createPattern(patternEnt, patternLHS, patternRHS);
			select = createParam(VARIABLE, "v");
			queryObj = createQueryObject(select, clause, pattern);
			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
		}

		TEST_METHOD(SelectParamNotInClauseTest) {
			Param select, lhs, rhs;
			Clause clause;
			Pattern pattern;
			QueryObject queryObj;

			/* stmt s; while w; Select w such that Follows(3, s) */
			lhs = createParam(INTEGER, "3");
			rhs = createParam(STMT, "s");
			clause = createClause(Parent, lhs, rhs);
			select = createParam(WHILE, "w");
			queryObj = createQueryObject(select, clause);
			Assert::AreEqual(false, evaluator.selectParamInClauses(queryObj));
		}


		TEST_METHOD(QueryHasClauseTest) {
			Param select = createParam(ASSIGN, "a");
			QueryObject qo = createQueryObject(select);

			//When there is no clause in the QueryObject
			Assert::AreNotEqual(true, evaluator.queryHasClause(qo));

			//When there is an existing clause in the QueryObject
			qo.insertClause(Parent, INTEGER, "2", STMT, "s");
			Assert::AreEqual(true, evaluator.queryHasClause(qo));
		}

		TEST_METHOD(QueryHasPatternTest) {
			Param select = createParam(STMT, "s");
			QueryObject qo = createQueryObject(select);

			//When there is no pattern in the QueryObject
			Assert::AreNotEqual(true, evaluator.queryHasPattern(qo));

			//When there is an existing clause in the QueryObject
			qo.insertPattern(ASSIGN, "a", IDENT, "x", VAR_NAME, "_\"x\"_");
			Assert::AreEqual(true, evaluator.queryHasPattern(qo));
		}

		TEST_METHOD(HasClauseResultsTest) {
			ClauseResults clauseResult1;
			clauseResult1.setValid(true);
			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult1));

			unordered_map<int, vector<int>> validKeyValues = { {4, { 6 } } };
			ClauseResults clauseResult2;
			clauseResult2.setkeyValues(validKeyValues);
			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult2));

			vector<int> validValues = { 4, 8 };
			ClauseResults clauseResult3;
			clauseResult3.setValues(validValues);
			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult3));

			ClauseResults invalidClauseResult;
			invalidClauseResult.setValid(false);
			Assert::AreNotEqual(true, evaluator.hasClauseResults(invalidClauseResult));
		}

		TEST_METHOD(StatementTypeToIntMapTest) {
			//Valid
			Assert::AreEqual(1, evaluator.statementTypeToIntMap(ASSIGN));
			Assert::AreEqual(2, evaluator.statementTypeToIntMap(WHILE));
			Assert::AreEqual(3, evaluator.statementTypeToIntMap(IF));

			// Invalid
			Assert::AreEqual(0, evaluator.statementTypeToIntMap(CONSTANT));
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