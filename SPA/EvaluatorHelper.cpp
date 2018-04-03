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

	vector<vector<int>> newTable;
	if (iTable.resultsTable.size() == 0) { // Table has no values
		for (vector<int> resultsRow : clauseResults.results) {
			newTable.push_back(resultsRow);
		}
	}
	else { // Update table for existing values
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
	}
	iTable.setResultsTable(newTable);
};

/* With overlapping synonyms
Possible optimization: Hash results / Sort merge tables
*/
void EvaluatorHelper::mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable) {

	int firstParamInt = iTable.getParamIndex(clauseResults.tableParams[0]);
	if (clauseResults.numParamsInResult() == 2) {
		int secondParamInt = iTable.getParamIndex(clauseResults.tableParams[1]);
		vector<vector<int>> newTable;
			
		for (vector<int> tableRow : iTable.resultsTable) {
			for (vector<int> resultRow : clauseResults.results) {
				int resultFirstParamValue = resultRow[0];
				int resultSecondParamValue = resultRow[1];
				if (firstParamInt > -1 && secondParamInt > -1) { // Both params in table
					if (resultFirstParamValue == tableRow[firstParamInt] &&
						resultSecondParamValue == tableRow[secondParamInt]) {
						newTable.push_back(tableRow);
						tableRow.pop_back();
					}
				} else if (firstParamInt > -1) { // First param in table
					if (resultFirstParamValue == tableRow[firstParamInt]) {
						tableRow.push_back(resultSecondParamValue); // Add in second param value
						newTable.push_back(tableRow);
						tableRow.pop_back();
					}
				} else { // Second param in table
					if (resultSecondParamValue == tableRow[secondParamInt]) {
						tableRow.push_back(resultFirstParamValue); // Add in first param value
						newTable.push_back(tableRow);
						tableRow.pop_back();
					}
				}
			}
		}
		iTable.setResultsTable(newTable);
	}
	else { // Only 1 synonym
		vector<vector<int>> newTable;
		for (vector<int> tableRow : iTable.resultsTable) {
			for (vector<int> resultRow : clauseResults.results) {
				int resultsParamValue = resultRow[0];
				if (resultsParamValue == tableRow[firstParamInt]) {
					newTable.push_back(tableRow);
				}
			}
		}
		iTable.setResultsTable(newTable);
	}

	/* Add table params */
	addClauseParamToTable(clauseResults, iTable);
};

/* Sort table according to specified param */
void EvaluatorHelper::sortTable(Param &p, IntermediateTable &iTable) {

	int paramIndex = iTable.getParamIndex(p);
	assert(paramIndex > -1); // Assert param exists in table
	
	/* Mergesort on table */
	vector<vector<int>> mergedResults = mergeSortResults(paramIndex, iTable.resultsTable);
	iTable.setResultsTable(mergedResults);
};

/* Merge sort for nested vector, specify index for comparison */
vector<vector<int>> EvaluatorHelper::mergeSortResults(int index, vector<vector<int>> &results) {
	if (left >= right) { // Single element do nothing
		return results;
	} else {
		int resultsSize = results.size();
		vector<vector<int>> leftArr;
		vector<vector<int>> rightArr;
		copy(results.begin(), results.begin() + (resultsSize / 2), back_inserter(leftArr)); // Copy first to middle
		copy(results.begin() + (resultsSize / 2) + 1, results.end(), back_inserter(rightArr)); // Copy middle + 1 to last
		mergeSortResults(index, results);
		mergeSortResults(index, results);

		vector<vector<int>> mergedResults;
		int leftIndex = 0;
		int rightIndex = resultsSize/2 + 1;
		while (leftIndex <= resultsSize / 2 && rightIndex <= resultsSize) {
			if (leftIndex > resultsSize / 2) { // Left array exhausted
				mergedResults.push_back(rightArr[rightIndex]);
				rightIndex++; 
				continue;
			}
			if (rightIndex > resultsSize) { // Right array exhausted
				mergedResults.push_back(leftArr[leftIndex]);
				leftIndex++; 
				continue;
			}
			if (leftArr[leftIndex][index] <= rightArr[rightIndex][index]) { // Left element smaller
				mergedResults.push_back(leftArr[leftIndex]);
				leftIndex++;
			}
			else { // Right element smaller
				mergedResults.push_back(rightArr[rightIndex]); 
				rightIndex++;
			}
		}

		return mergedResults;
	}
}

/* Merge Intermediate Tables 1 and 2 */
IntermediateTable EvaluatorHelper::mergeIntermediateTables(IntermediateTable &iTable1, IntermediateTable &iTable2) {

	for (pair<Param, int> paramInt : iTable2.tableParams) { // Check if table 2 is already merged
		if (iTable1.tableParams[paramInt.first] != -1) return iTable1;
	}

	int table1NumParams = iTable1.tableParams.size();
	// Add table2 params
	for (pair<Param, int> paramInt : iTable2.tableParams) {
		iTable1.tableParams[paramInt.first] = table1NumParams + paramInt.second;
	}

	// Cross product of two results tables
	vector<vector<int>> mergedResultsTable;
	for (vector<int> table1Row : iTable1.resultsTable) {
		for (vector<int> table2Row : iTable2.resultsTable) {
			vector<int> mergedRow = table1Row;
			mergedRow.insert(mergedRow.end(), table2Row.begin(), table2Row.end());
			mergedResultsTable.push_back(mergedRow);
		}
	}

	IntermediateTable mergedTable; mergedTable.instantiateTable();
	mergedTable.setTableParams(iTable1.tableParams);
	mergedTable.setResultsTable(mergedResultsTable);
	
	return mergedTable;
};

/* Returns true if param in clause result is in table */
bool EvaluatorHelper::clauseParamsInTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
	for (pair<Param, int> paramIndex : iTable.tableParams) {
		if (Utils::isSameParam(paramIndex.first, clauseResults.entRef)) return true;
		if (Utils::isSameParam(paramIndex.first, clauseResults.lhs)) return true;
		if (Utils::isSameParam(paramIndex.first, clauseResults.rhs)) return true;
	}
	return false;
};

/* Adds clauseResults params into table */
void EvaluatorHelper::addClauseParamToTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
	for (Param p : clauseResults.tableParams) {
		if (iTable.getParamIndex(p) == -1) { // Param does not exist in param table
			iTable.addTableParams(p);
		}
	}
};

/* Returns the pointer to the table containing the param */
IntermediateTable* EvaluatorHelper::findTableWithParam(Param p, vector<IntermediateTable> &iTables) {
	for (IntermediateTable iTable : iTables) {
		if (iTable.getParamIndex(p) != -1) {
			return &iTable;
		}
	}
	IntermediateTable emptyTable; map<Param, int> emptyTableParams; vector<vector<int>> emptyTableResults;
	emptyTable.setTableParams(emptyTableParams); emptyTable.setResultsTable(emptyTableResults);
	return &emptyTable;
}

/* Returns number of params of With Clause in table */
int EvaluatorHelper::withClauseNumSyns(Clause &clause, IntermediateTable &iTable) {
	int numSyns = 0;
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();
	if (iTable.getParamIndex(lhs) > -1) numSyns++; // Assume syn param in table
	if (iTable.getParamIndex(rhs) > -1) numSyns++; // Assume syn param in table
	return numSyns;
};

