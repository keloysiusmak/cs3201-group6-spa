#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"
#include "Utils.h"
#include "ClauseResults.h"

class Evaluator {
public:
	Evaluator();
	Evaluator(QueryObject);
	list<string> evaluateQuery();

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
	void evaluatePattern(Pattern &pattern, ClauseResults &patternResults);

	// Intersection Helpers
	void intersectSingle(ClauseResults &clauseResults);
	void intersectDouble(ClauseResults &clauseResults);
	vector<int> intersectVectors(vector<int> &v1, vector<int> &v2);
	list<string> intersectLists(list<string> &l1, list<string> &l2);
	unordered_map<int, vector<int>> consolidateKeyValues(vector<vector<int>> keyValues);

	// Helper Methods
	bool selectParamInClauses(QueryObject &queryObj);
	bool selectParamInClause(Param select, Clause &clause);
	bool selectParamInPattern(Param select, Pattern &pattern);
	bool queryHasClause(QueryObject &queryObj);
	bool queryHasPattern(QueryObject &queryObj);
	bool hasClauseResults(ClauseResults &clauseResults);
	list<string> getAllSelectedParam(Param p);
	list<string> resultToStringList(ClauseResults &clauseResults, Param &selected);

	vector<int> removeElems(vector<int> v1, vector<int> v2);
	vector<int> removeWhileIfs(vector<int> stmts);
	vector<int> getAllValuesFromMap(unordered_map<int, vector<int>> map);

	int statementTypeToIntMap(ParamType t);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	string invalidQueryMessage;
};