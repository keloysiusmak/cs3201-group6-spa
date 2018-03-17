#include "EvaluatorHelper.h"

using namespace std;

/* Merges the clauseResults into the intermediate table */
void EvaluatorHelper::mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
	if (clauseParamsInTable(clauseResults, iTable)) {
		mergeWithOverlap(clauseResults, iTable);
	}
	else {
		mergeWithoutOverlap(clauseResults, iTable);
	}
};

/* Cross Product when there are no overlaps */
void EvaluatorHelper::mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable) {
	/* Add table params */
	addClauseParamToTable(clauseResults, iTable);

	/* Update table */
	vector<vector<int>> newTable;
	for (vector<int> tableRow : iTable.resultsTable) {
		for (vector<int> resultsRow : clauseResults.results) {
			vector<int> newTableRow = tableRow;
			newTableRow.push_back(resultsRow[0]);
			if (clauseResults.numParamsInResult() == 2) { // Two syns
				newTableRow.push_back(resultsRow[1]);
			}
			newTable.push_back(newTableRow);
		}
	}
	iTable.setResultsTable(newTable);
};

/* With overlapping synonyms
Possible optimization: Hash results / Sort merge tables
*/
void EvaluatorHelper::mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable) {
	/* Add table params */
	addClauseParamToTable(clauseResults, iTable);

	int firstParamInt = getParamInt(clauseResults.tableParams[0], iTable);
	if (clauseResults.numParamsInResult() == 2) {
		int secondParamInt = getParamInt(clauseResults.tableParams[1], iTable);
		vector<vector<int>> newTable;
			
		for (vector<int> tableRow : iTable.resultsTable) {
			for (vector<int> resultRow : clauseResults.results) {

				if (firstParamInt >= 0 && secondParamInt >= 0) { // Both params in table
					if (resultRow[0] == tableRow[firstParamInt] &&
						resultRow[1] == tableRow[secondParamInt]) {
						newTable.push_back(tableRow);
					}
				} else if (firstParamInt >= 0) { // First param in table
					if (resultRow[0] == tableRow[firstParamInt]) {
						tableRow.push_back(resultRow[1]); // Add in second param value
						newTable.push_back(tableRow);
					}
				} else { // Second param in table
					if (resultRow[1] == tableRow[secondParamInt]) {
						tableRow.push_back(resultRow[0]); // Add in first param value
						newTable.push_back(tableRow);
					}
				}
			}
		}
	}
	else { // Only 1 synonym
		vector<vector<int>> newTable;
		for (vector<int> tableRow : iTable.resultsTable) {
			for (vector<int> resultRow : clauseResults.results) {
				if (resultRow[0] == tableRow[firstParamInt]) {
					newTable.push_back(tableRow);
				}
			}
		}
		iTable.setResultsTable(newTable);
	}
};

/* Returns true if param in clause result is in table */
bool EvaluatorHelper::clauseParamsInTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
	for (Param p : iTable.tableParams) {
		if (Utils::isSameParam(p, clauseResults.entRef)) return true;
		if (Utils::isSameParam(p, clauseResults.lhs)) return true;
		if (Utils::isSameParam(p, clauseResults.rhs)) return true;
	}
	return false;
};

/* Adds clauseResults params into table */
void EvaluatorHelper::addClauseParamToTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
	for (Param p : clauseResults.tableParams) {
		if (getParamInt(p, iTable) == -1) { // Param does not exist in param table
			iTable.addTableParams(p);
		}
	}
};

/* Returns index of param for intermediate table, if param does not exist, return -1 */
int EvaluatorHelper::getParamInt(Param p, IntermediateTable &iTable) {
	for (int i = 0; i < iTable.tableParams.size(); i++) {
		if (Utils::isSameParam(p, iTable.tableParams[i])) return i;
	}
	return -1;
};

