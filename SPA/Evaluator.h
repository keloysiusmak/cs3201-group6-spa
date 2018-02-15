#pragma once

#include <string>
#include <list>
#include "QueryObject.h"

struct ClauseResults {
	Param lhs;
	Param rhs;
	vector<string> lhsAnswers;
	vector<string> rhsAnswers;
};

class Evaluator {
public:
	Evaluator(QueryObject);
	list<string> evaluateQuery();
	ClauseResults evaluateClause(Clause);
	ClauseResults evaluateFollows(Clause);
	ClauseResults evaluateFollowStar(Clause);
private:
	QueryObject queryObject;
};
