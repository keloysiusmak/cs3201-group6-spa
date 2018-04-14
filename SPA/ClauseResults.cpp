#include "ClauseResults.h"
using namespace std;

void ClauseResults::instantiateClause(Clause clause) {
	valid = false;
	lhs = clause.getLeftParam();
	rhs = clause.getRightParam();
	if (Utils::isSynonym(lhs)) tableParams.push_back(lhs);
	if (Utils::isSynonym(rhs)) tableParams.push_back(rhs);
};

void ClauseResults::instantiatePattern(Pattern pattern) {
	valid = false;
	entRef = pattern.getEntity();
	lhs = pattern.getLeftParam();
	rhs = pattern.getRightParam();
	tableParams.push_back(entRef);
	if (Utils::isSynonym(lhs) && lhs.type != ALL) {
		tableParams.push_back(lhs);
	}
};

void ClauseResults::setResults(vector<vector<int>> &pkbResults) {
	results = pkbResults;
};

void ClauseResults::setValid(bool validity) {
	valid = validity;
};

void ClauseResults::clearResults() {
	vector<vector<int>> emptyResults = {};
	results = {};
};

void ClauseResults::clearParamsTable() {
	vector<Param> emptyTableParams = {};
	tableParams = {};
};

bool ClauseResults::isValid() {
	return valid;
};

/* Returns true if valid results exist */
bool ClauseResults::hasResults() {
	if (results.size() > 0) return true;
	if (valid) return true;
	return false;
};

/* Returns number of synonyms of results: 0/1/2 */
int ClauseResults::numParamsInResult() {
	return tableParams.size();
};

/* Remove ALL Syns */
void ClauseResults::removeALLSyns() {
	if (numParamsInResult() == 2) {
		Param leftParam = tableParams[0];
		Param rightParam = tableParams[1];

		vector<vector<int>> newTable;

		if (leftParam.type == ALL && rightParam.type == ALL) {
			if (results.size() > 0) setValid(true);
			clearResults();
			clearParamsTable();

		} else if (leftParam.type == ALL || rightParam.type == ALL) {

			clearParamsTable();
			if (leftParam.type == ALL) tableParams.push_back(rightParam);
			if (rightParam.type == ALL) tableParams.push_back(leftParam);

			for (vector<int> tableRow : results) {
				vector<int> newRow;
				if (leftParam.type == ALL) { // Left param is _
					newRow.push_back(tableRow[1]); }
				else { // Right param is _
					newRow.push_back(tableRow[0]); }
				newTable.push_back(newRow);
			}

			setResults(newTable);

		} else { ; }
	} else if (numParamsInResult() == 1) {
		Param param = tableParams[0];
		if (param.type == ALL) {
			if (results.size() > 0) setValid(true);
			clearResults();
			clearParamsTable();
		}
	} else { ; }
}


