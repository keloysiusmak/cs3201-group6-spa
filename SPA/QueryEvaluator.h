#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"
#include "Utils.h"
#include "ClauseResults.h"

class QueryEvaluator {
public:
	QueryEvaluator();
	QueryEvaluator(QueryObject);
	void evaluateQuery();

	bool isValidQuery();
	QueryObject getQueryObject();

	// Linkage to other components
	void setQueryObject(QueryObject);
	void setInvalidQuery(string);
	void setPKB(PKB);

	// Main Evaluation methods
	void evaluateClause(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollows(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollowStar(Clause &clause, ClauseResults &clauseResults);
	void evaluateParent(Clause &clause, ClauseResults &clauseResults);
	void evaluateParentStar(Clause &clause, ClauseResults &clauseResults);
	void evaluateUses(Clause &clause, ClauseResults &clauseResults);
	void evaluateModifies(Clause &clause, ClauseResults &clauseResults);
	void evaluateNext(Clause &clause, ClauseResults &clauseResults);
	void evaluateNextStar(Clause &clause, ClauseResults &clauseResults);
	void evaluateCalls(Clause &clause, ClauseResults &clauseResults);
	void evaluateCallsStar(Clause &clause, ClauseResults &clauseResults);
	void evaluatePattern(Pattern &pattern, ClauseResults &patternResults);

	//void getAllSelectedParam(Param p);

	// Helper Methods
	int queryNumClauses(QueryObject &queryObj);
	int queryNumPattern(QueryObject &queryObj);
	bool hasClauseResults(ClauseResults &clauseResults);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	string invalidQueryMessage;
};