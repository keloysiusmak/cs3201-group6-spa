#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "./PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {

	TEST_CLASS(EvaluatorTest) {

		TEST_METHOD(ParamInTableTest) {
			IntermediateTable table;

			/* Generate params */
			vector<Param> params;
			Param p1, p2, p3, p4;
			p1 = createParam(ASSIGN, "a1");
			p2 = createParam(ASSIGN, "a2");
			p3 = createParam(VARIABLE, "v");
			p4 = createParam(STMT, "s");
			params.push_back(p1); params.push_back(p2); params.push_back(p3); params.push_back(p4);

			/* 8 rows table */
			table.setResultsTable(createResultsTable(params, 8, table));
	
			/* 2 params in table */
			ClauseResults clauseWithTwoParamsInTable = createClauseResult(Uses, ASSIGN, "a2", VARIABLE, "v");
			Assert::AreEqual(true, EvaluatorHelper::paramInTable(clauseWithTwoParamsInTable, table));

			/* 1 params in table */
			ClauseResults clauseWithOneParamInTable = createClauseResult(Follows, ASSIGN, "a2", STMT, "s3");
			Assert::AreEqual(true, EvaluatorHelper::paramInTable(clauseWithOneParamInTable, table));

			/* No params in table */
			ClauseResults clauseWithNoParamInTable = createClauseResult(Follows, ASSIGN, "a3", STMT, "s3");
			Assert::AreEqual(false, EvaluatorHelper::paramInTable(clauseWithNoParamInTable, table));
		}

		TEST_METHOD(GetParamIntTest) {
			IntermediateTable table;

			/* Generate params */
			vector<Param> params;
			Param p1, p2, p3, p4;
			p1 = createParam(ASSIGN, "a1");
			p2 = createParam(ASSIGN, "a2");
			p3 = createParam(VARIABLE, "v");
			p4 = createParam(STMT, "s");
			params.push_back(p1); params.push_back(p2); params.push_back(p3); params.push_back(p4);

			/* 8 rows table */
			table.setResultsTable(createResultsTable(params, 8, table));
	
			/* second row index => 1 */
			Param validParamToBeSelected = createParam(ASSIGN, "a2");
			Assert::AreEqual(true, EvaluatorHelper::getParamInt(validParamToBeSelected, table) == 1);

			/* Param not in table returns -1 */
			Param invalidParamToBeSelected = createParam(WHILE, "w");
			Assert::AreEqual(true, EvaluatorHelper::getParamInt(invalidParamToBeSelected, table) == -1);
		}

		/* Randomly generates x rows of values for given params */
		vector<vector<int>> createResultsTable(vector<Param> params, int x, IntermediateTable &iTable) {
			iTable.setTableParams(params);
			vector<vector<int>> table;
			for (int i = 0; i < x; i++) {
				vector<int> row;
				for (Param p : params) {
					int randomInt = rand() % 100;
					row.push_back(randomInt);
				}
				table.push_back(row);
			}
			return table;
		}

		Param createParam(ParamType type, string value) {
			Param param;
			param.type = type;
			param.value = value;
			return param;
		};

		ClauseResults createClauseResult(RelRef rel, ParamType lhsType, string lhsValue, ParamType rhsType, string rhsValue) {
			Param lhs = createParam(lhsType, lhsValue);
			Param rhs = createParam(rhsType, rhsValue);
			Clause clause = Clause(rel, lhs, rhs);
			ClauseResults results;
			results.instantiateClause(clause);
			return results;
		};
	};
};