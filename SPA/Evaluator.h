#pragma once

#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"

struct ClauseResults {
	Param lhs;
	Param rhs;
	vector<string> lhsAnswers;
	vector<string> rhsAnswers;

	ClauseResults(Clause clause) {
		lhs = clause.getFirstParam();
		rhs = clause.getSecondParam();
	};
};

class Evaluator {
public:
	Evaluator();
	Evaluator(QueryObject);
	list<string> evaluateQuery();
	
	// Linkage to other components
	void setQueryObject(QueryObject);
	void setPKB(PKB);

	// Helper Evaluation Methods


	// PKB Evaluation methods
	ClauseResults evaluateClause(Clause);
	ClauseResults evaluateFollows(Clause);
	ClauseResults evaluateFollowStar(Clause);
	ClauseResults evaluateParent(Clause);
	ClauseResults evaluateParentStar(Clause);
private:
	QueryObject queryObject;
	PKB pkb;
};
