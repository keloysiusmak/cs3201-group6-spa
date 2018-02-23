#pragma once

#include <vector>
#include <string>
#include <list>
#include "QueryObject.h"
#include "../source/PKB.h"
#include "Utils.h"

struct ClauseResults {
	Param lhs;
	Param rhs;
	/* Key Values Hash for queries with 2 synonyms
		Vector of int values for queries with only 1 synonym */
	unordered_map<int, vector<int>> keyValues;
	vector<int> values;
	/* Boolean for when both params are concrete */
	bool valid;

	void ClauseResults::instantiateClause(Clause clause) {
		lhs = clause.getFirstParam();
		rhs = clause.getSecondParam();
	};

	void ClauseResults::setkeyValues(unordered_map<int, vector<int>> &results) {
		keyValues = results;
	}

	void ClauseResults::setkeyValues(vector<vector<int>> &results) {
		for (auto pair : results) {
			int key = pair.at(0);
			vector<int> value;
			value.push_back(pair.at(1));
			keyValues.insert({ key, value });
		}
	}

	void ClauseResults::setValues(vector<int> &results) {
		values = results;
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
	list<string> invalidQuery(string);

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
	void evaluatePattern(PatternObject &pattern, PatternResults &patternResults);
	list<string> resultToString(ClauseResults &clauseResults, Param &selected);

	// Intersection Helpers
	void intersectSingle(ClauseResults &clauseResults);
	void intersectDouble(ClauseResults &clauseResults);

	// Helper Methods
	int typeToIntMap(ParamType t);
	void storeMapToResults(ClauseResults &clauseResults, unordered_map<int, vector<int>> map);

private:
	QueryObject queryObject;
	PKB pkb;
	bool validQuery;
	string invalidQueryMessage;
};