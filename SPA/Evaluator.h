#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"

struct ClauseResults {
	Param lhs;
	Param rhs;
	vector<int> lhsAnswers;
	vector<int> rhsAnswers;
	bool valid;

	void ClauseResults::instantiateClause(Clause clause) {
		lhs = clause.getFirstParam();
		rhs = clause.getSecondParam();
	};

	void ClauseResults::setlhsAnswers(vector<int> ans) {
		lhsAnswers = ans;
	}

	void ClauseResults::setrhsAnswers(vector<int> ans) {
		rhsAnswers = ans;
	}

	void ClauseResults::setValid(bool validity) {
		valid = validity;
	}
};

struct PatternResults {
	Param lhs;
	Param rhs;
	vector<string> lhsAnswers;
	vector<string> rhsAnswers;

	void PatternResults::instantiatePattern(Pattern pattern) {
		lhs = pattern.getLeftParam();
		rhs = pattern.getRightParam();
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
	void evaluateClause(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollows(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollowStar(Clause &clause, ClauseResults &clauseResults);
	void evaluateParent(Clause &clause, ClauseResults &clauseResults);
	void evaluateParentStar(Clause &clause, ClauseResults &clauseResults);
private:
	QueryObject queryObject;
	PKB pkb;
};