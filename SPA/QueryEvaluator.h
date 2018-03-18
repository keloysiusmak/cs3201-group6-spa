#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"
#include "Utils.h"
#include "ClauseResults.h"
#include "EvaluatorHelper.h";

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

	// Helper Methods
	list<string> extractParams(vector<Param> selectedParams, IntermediateTable &iTable);
	list<string> paramToStringList(Param p, IntermediateTable &iTable);
	list<string> getAllParamsOfType(ParamType pType);
	string getProcOrVarName(AttrType type, int id);
	void handleWithClause(Clause &clause, IntermediateTable &iTable);
	void handleWithValueAssignment(Clause &clause, IntermediateTable &iTable);
	void handleWithEquateVariables(Clause &clause, IntermediateTable &iTable);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	list<string> invalidQueryMessage;
};
