#include "ClauseResults.h"
using namespace std;

void ClauseResults::instantiateClause(Clause clause) {
	lhs = clause.getFirstParam();
	rhs = clause.getSecondParam();
};

void ClauseResults::instantiatePattern(Pattern pattern) {
	entRef = pattern.getEntity();
	lhs = pattern.getLeftParam();
	rhs = pattern.getRightParam();
};

void ClauseResults::setkeyValues(unordered_map<int, vector<int>> &results) {
	keyValues = results;
};

void ClauseResults::setkeyValues(vector<vector<int>> &results) {
	for (auto pair : results) {
		int key = pair.at(0);
		vector<int> value;
		value.push_back(pair.at(1));
		keyValues.insert({ key, value });
	}
};

void ClauseResults::setValues(vector<int> &results) {
	values = results;
};

void ClauseResults::setValid(bool validity) {
	valid = validity;
};

void ClauseResults::setAssignmentsEnts(vector<int> &assigns) {
	assignmentsEnts = assigns;
};
