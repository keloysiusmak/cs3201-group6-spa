#pragma once
#include "ClauseResults.h"
#include "Utils.h"
#include "IntermediateTable.h"
#include <list>
#include <assert.h>

class EvaluatorHelper {

public:
	static Clause generalizeClause(Clause &clause);
	static void cacheUnsanitized(Clause &clause, ClauseResults &clauseResults, map<Clause, vector<vector<int>>> &cache);
	static void cacheSanitized(Clause &clause, ClauseResults &clauseResults, map<Clause, vector<vector<int>>> &cache);
	static bool unsanitizedClauseInCache(Clause &clause, map<Clause, vector<vector<int>>> &cache);
	static bool clauseInCache(Clause &clause, map<Clause, vector<vector<int>>> &cache);

	static void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);

	static void sortTable(Param &p, IntermediateTable &iTable);
	static vector<vector<int>> mergeSortResults(int index, vector<vector<int>> &results);

	static IntermediateTable mergeIntermediateTables(IntermediateTable &iTable1, IntermediateTable &iTable2);

	static bool clauseParamsInTable(Clause &clause, IntermediateTable &iTable);
	static bool clauseResultsParamsInTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void addClauseParamToTable(ClauseResults &clauseResults, IntermediateTable &iTable);

	static IntermediateTable findTableWithParam(Param p, vector<IntermediateTable> &iTables);

	static int withClauseNumSyns(Clause &clause, IntermediateTable &iTable);

	/* Extension Helpers */
	static vector<vector<int>> crossVectors(vector<vector<int>> &set1, vector<vector<int>> &set2);
	static void subtractSet(Param p, ClauseResults &clauseResults, vector<vector<int>> &setToSubtract);
};
