#pragma once

#include <vector>
#include <string>
#include <list>
#include <set>
#include "QueryObject.h"
#include "PKB.h"
#include "Utils.h"
#include "ClauseResults.h"
#include "EvaluatorHelper.h";
#include "QueryOptimization.h"
#include "IntermediateTable.h"

class QueryEvaluator {
public:
	QueryEvaluator();
	QueryEvaluator(QueryObject);
	list<string> evaluateQuery();

	bool isValidQuery();
	QueryObject getQueryObject();

	// Linkage to other components
	void setQueryObject(QueryObject);
	void setPKB(PKB);

	// Main Evaluation methods
	void evaluateClauseGeneral(Clause &clause, ClauseResults &clauseResults,
		IntermediateTable &iTable, map<Clause, vector<vector<int>>> &cache);

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
	void evaluateAffects(Clause &clause, ClauseResults &clauseResults);
	//void evaluateAffectsStar(Clause &clause, ClauseResults &clauseResults);
	void evaluatePattern(Pattern &pattern, ClauseResults &patternResults);

	// Evaluation Helpers
	void filterStmts(ClauseResults &clauseResults);
	set<int> getParamSet(Param p);

	// Printing Helpers
	list<string> extractParams(vector<Param> selectedParams, vector<IntermediateTable> &iTables);
	list<string> paramToStringList(Param p, IntermediateTable &iTable);
	list<string> getAllParamsOfType(Param p);
	int getId(Param p, ParamType type, AttrType attribute);
	string getProcOrVarName(AttrType type, int id);

	// With Clause Helpers
	void handleWithClause(Clause &clause, IntermediateTable &iTable);
	void handleWithValueAssignment(Clause &clause, IntermediateTable &iTable);
	void handleWithEquateVariables(Clause &clause, IntermediateTable &iTable);
	bool handleWithEvaluation(Clause &clause, IntermediateTable &iTable);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	list<string> invalidQueryMessage;
};