#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "./PKB.h"

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

		TEST_METHOD(EvaluatorInvalidQueryObjectSetter) {
			evaluator.setInvalidQuery({});
			Assert::AreEqual(false, evaluator.isValidQuery());
		};

//		TEST_METHOD(EvaluatorSelectParamInClause) {
//			Param select;
//			Clause clause;
//			Pattern pattern;
//			QueryObject queryObj;
//
//			/* stmt s; Select s such that Follows(1, s) */
//			clause = createClause(Follows, INTEGER, "1", STMT, "s");
//			select = createParam(STMT, "s");
//			queryObj = createQueryObject(select, clause);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* while w; Select w such that Follows(w, 2) */
//			clause = createClause(Follows, WHILE, "w", INTEGER, "2");
//			select = createParam(WHILE, "w");
//			queryObj = createQueryObject(select, clause);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* stmt s; while w; Select w such that Follows(w, s) */
//			clause = createClause(Follows, WHILE, "w", STMT, "s");
//			select = createParam(WHILE, "w");
//			queryObj = createQueryObject(select, clause);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* if ifs; Select ifs such that Follows(ifs, 2) */
//			clause = createClause(Follows, IF, "ifs", INTEGER, "2");
//			select = createParam(IF, "ifs");
//			queryObj = createQueryObject(select, clause);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* variable v; Select w such that Follows(1, 2)pattern(v, 1) */
//			clause = createClause(Follows, INTEGER, "1", INTEGER, "2");
//			pattern = createPattern(ASSIGN, "a", VARIABLE, "v", CONSTANT, "1");
//			select = createParam(VARIABLE, "v");
//			queryObj = createQueryObject(select, clause, pattern);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* stmt s; while w; Select w such that Follows(1, 2)pattern("c", v) */
//			clause = createClause(Follows, INTEGER, "1", INTEGER, "2");
//			pattern = createPattern(ASSIGN, "a", VAR_NAME, "c", VARIABLE, "v");
//			select = createParam(VARIABLE, "v");
//			queryObj = createQueryObject(select, clause, pattern);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//		};
//
//		TEST_METHOD(EvaluatorSelectParamInPattern) {
//			Param select, clauseLHS, clauseRHS, patternEnt, patternLHS, patternRHS;
//			Clause clause;
//			Pattern pattern;
//			QueryObject queryObj;
//
//			/* variable v; pattern a("c", _) */
//			pattern = createPattern(ASSIGN, "a", VARIABLE, "v", ALL, "_");
//			select = createParam(VARIABLE, "v");
//			queryObj = createQueryObject(select, clause, pattern);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* variable v; Select v such that Parent(1, 2) pattern a("c", _) */
//			clause = createClause(Parent, INTEGER, "1", INTEGER, "2");
//			pattern = createPattern(ASSIGN, "a", VAR_NAME, "c", ALL, "_");
//			select = createParam(ASSIGN, "a");
//			queryObj = createQueryObject(select, clause, pattern);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//
//			/* Select v such that Modifies(1, v) pattern a(v, _"x"_) */
//			clause = createClause(Modifies, INTEGER, "1", VARIABLE, "v");
//			pattern = createPattern(ASSIGN, "a", VARIABLE, "v", VAR_NAME, "x");
//			select = createParam(VARIABLE, "v");
//			queryObj = createQueryObject(select, clause, pattern);
//			Assert::AreEqual(true, evaluator.selectParamInClauses(queryObj));
//		};
//
//		TEST_METHOD(EvaluatorSelectParamNotInClause) {
//			Param select, lhs, rhs;
//			Clause clause;
//			Pattern pattern;
//			QueryObject queryObj;
//
//			/* stmt s; while w; Select w such that Follows(3, s) */
//			lhs = createParam(INTEGER, "3");
//			rhs = createParam(STMT, "s");
//			clause = createClause(Parent, INTEGER, "3", STMT, "s");
//			select = createParam(WHILE, "w");
//			queryObj = createQueryObject(select, clause);
//			Assert::AreEqual(false, evaluator.selectParamInClauses(queryObj));
//		};
//
//		TEST_METHOD(EvaluatorQueryHasClause) {
//			Param select = createParam(ASSIGN, "a");
//			QueryObject queryObject = createQueryObject(select);
//
//			// Clause missing
//			Assert::AreEqual(false, evaluator.queryHasClause(queryObject));
//
//			// Clause present
//			queryObject.insertClause(Parent, INTEGER, "2", STMT, "s");
//			Assert::AreEqual(true, evaluator.queryHasClause(queryObject));
//		};
//
//		TEST_METHOD(EvaluatorQueryHasPattern) {
//			Param select = createParam(STMT, "s");
//			QueryObject queryObject = createQueryObject(select);
//
//			// Pattern missing
//			Assert::AreNotEqual(true, evaluator.queryHasPattern(queryObject));
//
//			// Pattern present
//			queryObject.insertPattern(ASSIGN, "a", IDENT, "x", VAR_NAME, "_\"x\"_");
//			Assert::AreEqual(true, evaluator.queryHasPattern(queryObject));
//		};
//
//		TEST_METHOD(EvaluatorHasClauseResults) {
//			// Validity check
//			ClauseResults clauseResult1;
//			clauseResult1.setValid(true);
//			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult1));
//
//			// keyValues check
//			unordered_map<int, vector<int>> validKeyValues = { {4, { 6 } } };
//			ClauseResults clauseResult2;
//			clauseResult2.setkeyValues(validKeyValues);
//			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult2));
//
//			// values check
//			vector<int> validValues = { 4, 8 };
//			ClauseResults clauseResult3;
//			clauseResult3.setValues(validValues);
//			Assert::AreEqual(true, evaluator.hasClauseResults(clauseResult3));
//
//			ClauseResults invalidClauseResult;
//			invalidClauseResult.setValid(false);
//			Assert::AreEqual(false, evaluator.hasClauseResults(invalidClauseResult));
//		};
//
//		TEST_METHOD(EvaluatorStatementTypeToIntMap) {
//			// Assigned Types
//			Assert::AreEqual(1, evaluator.statementTypeToIntMap(ASSIGN));
//			Assert::AreEqual(2, evaluator.statementTypeToIntMap(WHILE));
//			Assert::AreEqual(3, evaluator.statementTypeToIntMap(IF));
//
//			// Other types
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(ALL));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(STMT));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(PROG_LINE));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(VARIABLE));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(CONSTANT));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(VAR_NAME));
//			Assert::AreEqual(0, evaluator.statementTypeToIntMap(IDENT));
//		};
//
//		TEST_METHOD(EvaluatorResultToStringList) {
//			Param select;
//			Clause clause; Pattern pattern;
//			ClauseResults clauseResults;
//			unordered_map<int, vector<int>> keyValues;
//			vector<int> values;
//			vector<int> assignEnts;
//			list<string> expected;
//
//			// Keys of keyValues
//			keyValues = { {1, {2, 3, 4}} };
//			select = createParam(ASSIGN, "a");
//			clause = createClause(Follows, ASSIGN, "a", WHILE, "w");
//			clauseResults.instantiateClause(clause);
//			clauseResults.setkeyValues(keyValues);
//			expected = { "1" };
//			Assert::AreEqual(true, expected == evaluator.resultToStringList(clauseResults, select));
//
//			// Values of keyValues
//			select = createParam(WHILE, "w");
//			clause = createClause(Follows, ASSIGN, "a", WHILE, "w");
//			clauseResults.instantiateClause(clause);
//			clauseResults.setkeyValues(keyValues);
//			expected = { "2", "3", "4" };
//			Assert::AreEqual(true, expected == evaluator.resultToStringList(clauseResults, select));
//
//			// Values
//			select = createParam(WHILE, "w");
//			clause = createClause(Follows, STMT, "1", WHILE, "w");
//			clauseResults.instantiateClause(clause);
//			values = { 1, 2 };
//			clauseResults.setValues(values);
//			expected = { "1", "2" };
//			Assert::AreEqual(true, expected == evaluator.resultToStringList(clauseResults, select));
//
//			// assignEnts
//			select = createParam(STMT, "s");
//			pattern = createPattern(ASSIGN, "a", VAR_NAME, "x", CONSTANT, "1");
//			clauseResults.instantiatePattern(pattern);
//			assignEnts = { 1, 2 };
//			clauseResults.setAssignmentsEnts(assignEnts);
//			expected = { "1", "2" };
//			Assert::AreEqual(true, expected == evaluator.resultToStringList(clauseResults, select));
//		};
//
//		TEST_METHOD(EvaluatorGetAllValuesFromMap) {
//			unordered_map<int, vector<int>> map = { {1, {2, 3}}, {2, {3, 5}} };
//			vector<int> values = evaluator.getAllValuesFromMap(map);
//			vector<int> expected = { 2, 3, 5 };
//			Assert::AreEqual(true, expected == values);
//		};
//
//		TEST_METHOD(EvaluatorRemoveElems) {
//			vector<int> v1 = { 1, 2, 3, 4, 5 };
//			vector<int> v2 = { 4, 5 };
//			vector<int> expected = { 1, 2, 3 };
//			Assert::AreEqual(true, expected == evaluator.removeElems(v1, v2));
//		};
//
//		TEST_METHOD(EvaluatorIntersectVectors) {
//			vector<int> v1 = { 1, 2, 3, 4, 5 };
//			vector<int> v2 = { 4, 5, 6 };
//			vector<int> expected = { 4, 5 };
//			Assert::AreEqual(true, expected == evaluator.intersectVectors(v1, v2));
//		};
//
//		TEST_METHOD(EvaluatorIntersectLists) {
//			list<string> strList1 = { "1", "2", "3", "4" };
//			list<string> strList2 = { "3", "5", "1", "6" };
//			list<string> expected = { "1", "3" };
//			Assert::AreEqual(true, expected == evaluator.intersectLists(strList1, strList2));
//		}
//
//		TEST_METHOD(EvaluatorConsolidateKeyValues) {
//			vector<vector<int>> keyValuePairs = { {1, 2}, {1, 3}, {1, 4}, {2, 5}, {2,7} };
//			unordered_map<int, vector<int>> consolidated = { {1, {2, 3, 4}}, {2, {5, 7}} };
//			Assert::AreEqual(true, consolidated == evaluator.consolidateKeyValues(keyValuePairs));
//
//		}
//
//		/* Object creation helpers*/
//		Param createParam(ParamType type, string value) {
//			Param param;
//			param.type = type;
//			param.value = value;
//			return param;
//		};
//
//		Clause createClause(RelRef rel, ParamType lhsType, string lhsValue, ParamType rhsType, string rhsValue) {
//			Param lhs = createParam(lhsType, lhsValue);
//			Param rhs = createParam(rhsType, rhsValue);
//			return Clause(rel, lhs, rhs);
//		};
//
//		Pattern createPattern(ParamType entType, string entValue, ParamType lhsType, string lhsValue, ParamType rhsType, string rhsValue) {
//			Param ent = createParam(entType, entValue);
//			Param lhs = createParam(lhsType, lhsValue);
//			Param rhs = createParam(rhsType, rhsValue);
//			return Pattern(ent, lhs, rhs);
//		}
//
//		/* Overloaded QueryObject constructor methods */
//
//		/* Only select statement */
//		QueryObject createQueryObject(Param select) {
//			QueryObject queryObj;
//			queryObj.insertSelectStmt(select.type, select.value);
//			return queryObj;
//		};
//
//		/* Clause present */
//		QueryObject createQueryObject(Param select, Clause clause) {
//			QueryObject queryObj;
//			queryObj.insertSelectStmt(select.type, select.value);
//			queryObj.insertClause(clause.getRelRef(), clause.getFirstParam().type,
//				clause.getFirstParam().value, clause.getSecondParam().type, clause.getSecondParam().value);
//			return queryObj;
//		};
//
//		/* Clause and Pattern present */
//		QueryObject createQueryObject(Param select, Clause clause, Pattern pattern) {
//			QueryObject queryObj;
//			queryObj.insertSelectStmt(select.type, select.value);
//			queryObj.insertClause(clause.getRelRef(), clause.getFirstParam().type,
//				clause.getFirstParam().value, clause.getSecondParam().type, clause.getSecondParam().value);
//			queryObj.insertPattern(pattern.getEntity().type, pattern.getEntity().value,
//				pattern.getLeftParam().type, pattern.getLeftParam().value,
//				pattern.getRightParam().type, pattern.getRightParam().value);
//			return queryObj;
//		};
	};
}