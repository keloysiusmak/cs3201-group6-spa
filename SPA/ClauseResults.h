#pragma once
#include <unordered_map>
#include "Clause.h"
#include "Pattern.h"

class ClauseResults {
public:
	Param lhs;
	Param rhs;
	/* Key Values Hash for queries with 2 synonyms
		Vector of int values for queries with only 1 synonym */
	unordered_map<int, vector<int>> keyValues;
	vector<int> values;
	/* Boolean for when both params are concrete */
	bool valid;
	/* Solely used for pattern */
	Param entRef;
	vector<int> assignmentsEnts;

	// Instantiation
	void ClauseResults::instantiateClause(Clause clause);
	void ClauseResults::instantiatePattern(Pattern pattern);

	// Setter methods
	void ClauseResults::setkeyValues(unordered_map<int, vector<int>> &results);
	void ClauseResults::setkeyValues(vector<vector<int>> &results);
	void ClauseResults::setValues(vector<int> &results);
	void ClauseResults::setValid(bool validity);
	void ClauseResults::setAssignmentsEnts(vector<int> &assigns);
};
