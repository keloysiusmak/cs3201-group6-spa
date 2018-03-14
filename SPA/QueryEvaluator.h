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
	list<string> evaluateQuery();

	bool isValidQuery();
	QueryObject getQueryObject();

	// Linkage to other components
	void setQueryObject(QueryObject);
	void setInvalidQuery(string);
	void setPKB(PKB);

	// Main Evaluation methods
	vector<vector<int>> evaluateClause(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateFollows(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateFollowStar(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateParent(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateParentStar(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateUses(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateModifies(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateNext(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateNextStar(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateCalls(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluateCallsStar(Clause &clause, ClauseResults &clauseResults);
	vector<vector<int>> evaluatePattern(Pattern &pattern, ClauseResults &patternResults);

	// Helper Methods
	bool queryHasClause(QueryObject &queryObj);
	bool queryHasPattern(QueryObject &queryObj);
	bool hasClauseResults(ClauseResults &clauseResults);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	string invalidQueryMessage;
};