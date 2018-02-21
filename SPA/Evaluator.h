#pragma once

#include <string>
#include <list>
#include "QueryObject.h"

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
	ClauseResults evaluateClause(Clause);
	ClauseResults evaluateFollows(Clause);
	ClauseResults evaluateFollowStar(Clause);
	ClauseResults evaluateParent(Clause);
	ClauseResults evaluateParentStar(Clause);
private:
	QueryObject queryObject;
};
