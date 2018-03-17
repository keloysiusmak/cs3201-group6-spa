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
			Assert::AreEqual(true, EvaluatorHelper::clauseParamsInTable(clauseWithTwoParamsInTable, table));

			/* 1 params in table */
			ClauseResults clauseWithOneParamInTable = createClauseResult(Follows, ASSIGN, "a2", STMT, "s3");
			Assert::AreEqual(true, EvaluatorHelper::clauseParamsInTable(clauseWithOneParamInTable, table));

			/* No params in table */
			ClauseResults clauseWithNoParamInTable = createClauseResult(Follows, ASSIGN, "a3", STMT, "s3");
			Assert::AreEqual(false, EvaluatorHelper::clauseParamsInTable(clauseWithNoParamInTable, table));
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

		TEST_METHOD(MergeWithoutOverlapOneParamTest) {
			/* Instantiate table*/
			IntermediateTable table;

			/* Generate params and initial table values */
			vector<Param> params;
			Param p1, p2, p3, p4;
			p1 = createParam(ASSIGN, "a1");
			p2 = createParam(ASSIGN, "a2");
			p3 = createParam(VARIABLE, "v");
			p4 = createParam(STMT, "s");
			params.push_back(p1); params.push_back(p2); params.push_back(p3); params.push_back(p4);

			vector<vector<int>> resultsBeforeMerge = {
				{1, 2, 3, 4},
				{2, 3, 4, 5},
				{3, 4, 5, 6},
				{4, 5, 6, 7}
			};

			table.setTableParams(params);
			table.setResultsTable(resultsBeforeMerge);

			/* ClauseResults instantiation */
			ClauseResults clauseResults = createClauseResult(Follows, STMT, "s2", INTEGER, "5");
			vector<vector<int>> clauseResultsTable = { {1}, {2} };
			clauseResults.setResults(clauseResultsTable);

			/* Final params and results table after merge */
			vector<vector<int>> resultsAfterMerge = {
				{1, 2, 3, 4, 1},
				{1, 2, 3, 4, 2},
				{2, 3, 4, 5, 1},
				{2, 3, 4, 5, 2},
				{3, 4, 5, 6, 1},
				{3, 4, 5, 6, 2},
				{4, 5, 6, 7, 1},
				{4, 5, 6, 7, 2},
			};
			Param p5 = createParam(STMT, "s2");
			vector<Param> updatedParams;
			updatedParams.push_back(p1); updatedParams.push_back(p2); updatedParams.push_back(p3);
			updatedParams.push_back(p4); updatedParams.push_back(p5);

			/* Merge and test */
			EvaluatorHelper::mergeWithoutOverlap(clauseResults, table);

			for (size_t i = 0; i < resultsAfterMerge.size(); i++) {
				for (size_t j = 0; j < resultsAfterMerge[i].size(); j++) {
					Assert::AreEqual(true, table.resultsTable[i][j] == resultsAfterMerge[i][j]);
				}
			}
			
			for (size_t k = 0; k < updatedParams.size(); k++) {
				Assert::AreEqual(true, Utils::isSameParam(table.tableParams[k], updatedParams[k]));
			}
		}

		TEST_METHOD(MergeWithoutOverlapTwoParamsTest) {
			/* Instantiate table*/
			IntermediateTable table;

			/* Generate params and initial table values */
			vector<Param> params;
			Param p1, p2, p3, p4;
			p1 = createParam(ASSIGN, "a1");
			p2 = createParam(ASSIGN, "a2");
			p3 = createParam(VARIABLE, "v");
			p4 = createParam(STMT, "s");
			params.push_back(p1); params.push_back(p2); params.push_back(p3); params.push_back(p4);

			vector<vector<int>> resultsBeforeMerge = {
				{1, 2, 3, 4},
				{2, 3, 4, 5},
				{3, 4, 5, 6},
				{4, 5, 6, 7}
			};

			table.setTableParams(params);
			table.setResultsTable(resultsBeforeMerge);

			/* ClauseResults Instantiation */
			ClauseResults clauseResults = createClauseResult(Follows, STMT, "s2", STMT, "s3");
			vector<vector<int>> clauseResultsTable = { {1, 3}, {2, 4} };
			clauseResults.setResults(clauseResultsTable);

			/* Final params and results table after merge */
			vector<vector<int>> resultsAfterMerge = {
				{1, 2, 3, 4, 1, 3},
				{1, 2, 3, 4, 2, 4},
				{2, 3, 4, 5, 1, 3},
				{2, 3, 4, 5, 2, 4},
				{3, 4, 5, 6, 1, 3},
				{3, 4, 5, 6, 2, 4},
				{4, 5, 6, 7, 1, 3},
				{4, 5, 6, 7, 2, 4},
			};
			Param p5 = createParam(STMT, "s2");
			Param p6 = createParam(STMT, "s3");
			vector<Param> updatedParams;
			updatedParams.push_back(p1); updatedParams.push_back(p2); updatedParams.push_back(p3);
			updatedParams.push_back(p4); updatedParams.push_back(p5); updatedParams.push_back(p6);

			/* Merge */
			EvaluatorHelper::mergeWithoutOverlap(clauseResults, table);

			for (size_t i = 0; i < resultsAfterMerge.size(); i++) {
				for (size_t j = 0; j < resultsAfterMerge[i].size(); j++) {
					Assert::AreEqual(true, table.resultsTable[i][j] == resultsAfterMerge[i][j]);
				}
			}

			for (size_t k = 0; k < updatedParams.size(); k++) {
				Assert::AreEqual(true, Utils::isSameParam(table.tableParams[k], updatedParams[k]));
			}
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
