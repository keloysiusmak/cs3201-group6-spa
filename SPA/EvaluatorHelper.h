#pragma once
#include "ClauseResults.h"
#include "Utils.h"
#include "IntermediateTable.h"
#include <list>


class EvaluatorHelper {

public:
	static void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);

	static IntermediateTable mergeIntermediateTables(IntermediateTable &iTable1, IntermediateTable &iTable2);

	static bool clauseParamsInTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void addClauseParamToTable(ClauseResults &clauseResults, IntermediateTable &iTable);

	static IntermediateTable* findTableWithParam(Param p, vector<IntermediateTable> &iTables);

	static int withClauseNumSyns(Clause &clause, IntermediateTable &iTable);
};
