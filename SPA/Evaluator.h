#pragma once

#include <string>
#include <list>
#include "QueryObject.h"

using namespace std;

class Evaluator {
public:
	Evaluator(QueryObject queryObj);
	list<string> evaluateQuery();
	ClauseResults evaluateClause(Clause);
	ClauseResults evaluateFollows();
	ClauseResults evaluateFollowsStar();
private:
	QueryObject queryObject;
};

struct ClauseResults {
	string lhsType;
	string rhsType;
	string lhsArg;
	string rhsArg;
	vector<string> lhsAnswers;
	vector<string> rhsAnswers;
};