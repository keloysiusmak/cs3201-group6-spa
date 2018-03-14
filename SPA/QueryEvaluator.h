#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"
#include "Utils.h"
#include "ClauseResults.h"

class QueryEvaluator {
<<<<<<< HEAD
 public:
  QueryEvaluator();
  QueryEvaluator(QueryObject);
  vector<vector<int>> evaluateQuery();

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

  vector<vector<int>> getAllSelectedParam(Param p);

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
=======
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
>>>>>>> c48ad9dedc5e0e9e6154b68108bd5a40e8191eea
