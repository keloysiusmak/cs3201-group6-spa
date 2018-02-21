#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"

struct ClauseResults {
	Param lhs;
	Param rhs;
	/* values are only set if both params are synonyms, otherwise only keys are set to represent 
	corresponding possible values wrt either lhs||rhs constant*/
	vector<int> keys;
	vector<vector<int>> values;
	bool valid;

	void ClauseResults::instantiateClause(Clause clause) {
		lhs = clause.getFirstParam();
		rhs = clause.getSecondParam();
	};

	void ClauseResults::setKeys(vector<int> ans) {
		keys = ans;
	}

	void ClauseResults::setValues(vector<vector<int>> ans) {
		values = ans;
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

	// Main Evaluation methods
	void evaluateClause(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollows(Clause &clause, ClauseResults &clauseResults);
	void evaluateFollowStar(Clause &clause, ClauseResults &clauseResults);
	void evaluateParent(Clause &clause, ClauseResults &clauseResults);
	void evaluateParentStar(Clause &clause, ClauseResults &clauseResults);
	list<string> resultToString(ClauseResults &clauseResults, Param &selected);

	// Helper Evaluation Methods
	void storeMapToResults(ClauseResults &clauseResults, unordered_map<int, vector<int>> map);

private:
	QueryObject queryObject;
	PKB pkb;
};