#include "EvaluatorHelper.h"

using namespace std;

/* Generalize clause by making all synonyms ALL */
Clause EvaluatorHelper::generalizeClause(Clause &clause) {
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();

	Clause generalizedClause;
	generalizedClause.setRelRef(clause.getRelRef());

	if (Utils::isSynonym(lhs)) {
		Param cachedLHS = Utils::createParam(ALL, "_", NONE);
		generalizedClause.setLHS(cachedLHS);
	}
	else generalizedClause.setLHS(lhs);

	if (Utils::isSynonym(rhs)) {
		Param cachedRHS = Utils::createParam(ALL, "_", NONE);
		generalizedClause.setRHS(cachedRHS);
	}
	else generalizedClause.setRHS(rhs);

	return generalizedClause;
};

/* Store the unsanitized results of the clause, i.e. replacing syn with ALL*/
void EvaluatorHelper::cacheUnsanitized(Clause &clause, ClauseResults &clauseResults, map<Clause, vector<vector<int>>> &cache) {
	Clause cachedClause = generalizeClause(clause);
	cache[cachedClause] = clauseResults.results;
};

/* Store actual results of clause in cache */
void EvaluatorHelper::cacheSanitized(Clause &clause, ClauseResults &clauseResults, map<Clause, vector<vector<int>>> &cache) {
	cache[clause] = clauseResults.results;
};

/* Checks if actual clause is in cache */
bool EvaluatorHelper::clauseInCache(Clause &clause, map<Clause, vector<vector<int>>> &cache) {
	return cache.find(clause) != cache.end();
};

/* Checks if unsanitized clause is in cache */
bool EvaluatorHelper::unsanitizedClauseInCache(Clause &clause, map<Clause, vector<vector<int>>> &cache) {
	Clause potentialCachedClause = generalizeClause(clause);
	return cache.find(potentialCachedClause) != cache.end();
};

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

	/* Set iTable as hasResults if clauseResults is (concrete, concrete) and true */
	if (clauseResults.isValid()) iTable.hasResults = true;

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

	if (clauseResults.numParamsInResult() == 2) { // Two synonyms

		/* 0: both params in table
		1: left param in table
		2: right param in table */
		int paramsInTableCase;

		Param leftParam = clauseResults.tableParams[0];
		Param rightParam = clauseResults.tableParams[1];
		int leftParamIndex = iTable.getParamIndex(leftParam);
		int rightParamIndex = iTable.getParamIndex(rightParam);

		if (leftParamIndex >= 0 && rightParamIndex >= 0) { // If both params are in table
			paramsInTableCase = 0;
			clauseResults.setResults(mergeSortResults(0, mergeSortResults(1, clauseResults.results)));
			sortTable(rightParam, iTable); sortTable(leftParam, iTable);

		} else if (leftParamIndex >= 0) { // Left param in table
			paramsInTableCase = 1;
			clauseResults.setResults(mergeSortResults(0, clauseResults.results));
	  		sortTable(leftParam, iTable);

		} else { // Right param in table
			paramsInTableCase = 2;
			clauseResults.setResults(mergeSortResults(1, clauseResults.results));
			sortTable(rightParam, iTable);
		}

		vector<vector<int>> mergedResults;
		int tableResultsIndex = 0;
		int clauseResultsIndex = 0;
			
		while (tableResultsIndex < iTable.resultsTable.size() && clauseResultsIndex < clauseResults.results.size()) {

			if (paramsInTableCase == 0) { // Both params in table
				int tableLeftParamValue = iTable.resultsTable[tableResultsIndex][leftParamIndex];
				int tableRightParamValue = iTable.resultsTable[tableResultsIndex][rightParamIndex];
				int clauseLeftParamValue = clauseResults.results[clauseResultsIndex][0];
				int clauseRightParamValue = clauseResults.results[clauseResultsIndex][1];

				if (tableLeftParamValue == clauseLeftParamValue) { // left param equals

					if (tableRightParamValue == clauseRightParamValue) {
						mergedResults.push_back(iTable.resultsTable[tableResultsIndex]);
						tableResultsIndex++; clauseResultsIndex++;
					} else if (tableRightParamValue > clauseRightParamValue) {
						clauseResultsIndex++;
					} else {
						tableResultsIndex++;
					}
				} else {
					(tableLeftParamValue < clauseLeftParamValue) ?
						clauseResultsIndex++ : tableResultsIndex++;
				}
				
			} else if (paramsInTableCase == 1) { // Left param in table

				int tableLeftParamValue = iTable.resultsTable[tableResultsIndex][leftParamIndex];
				int clauseLeftParamValue = clauseResults.results[clauseResultsIndex][0];
				
				if (tableLeftParamValue == clauseLeftParamValue) {
					int tableIndex = tableResultsIndex;
					vector<int> tableRow;
					while (iTable.resultsTable[tableIndex][leftParamIndex] == clauseLeftParamValue) {
						tableRow = iTable.resultsTable[tableResultsIndex];
						tableRow.push_back(clauseResults.results[clauseResultsIndex][1]);
						tableIndex++;
						mergedResults.push_back(tableRow);
					}
					tableResultsIndex++;
					clauseResultsIndex++;

				} else {
					(tableLeftParamValue < clauseLeftParamValue) ?
						clauseResultsIndex++ : tableResultsIndex++;
				}

			} else { // Right param in table

				int tableRightParamValue = iTable.resultsTable[tableResultsIndex][rightParamIndex];
				int clauseRightParamValue = clauseResults.results[clauseResultsIndex][1];
				
				if (tableRightParamValue == clauseRightParamValue) {

					int tableIndex = tableResultsIndex++;
					vector<int> tableRow;
					while (iTable.resultsTable[tableIndex][rightParamIndex] == clauseRightParamValue) {
						tableRow = iTable.resultsTable[tableResultsIndex];
						tableRow.push_back(clauseResults.results[clauseResultsIndex][0]);
						tableIndex++;
					}
					tableResultsIndex++;
					clauseResultsIndex++;

				} else {
					(tableRightParamValue < clauseRightParamValue) ?
						clauseResultsIndex++ : tableResultsIndex++;
				}

			}
		}

		iTable.setResultsTable(mergedResults);

	} else { // Only 1 synonym
		Param paramToSortBy = clauseResults.tableParams[0];
		int paramIndex = iTable.getParamIndex(paramToSortBy);

		// Sort clauseResults and table results to facilitate merging
		clauseResults.setResults(mergeSortResults(0, clauseResults.results));
		sortTable(paramToSortBy, iTable);

		vector<vector<int>> mergedResults;
		int tableResultsIndex = 0;
		int clauseResultsIndex = 0;
		// Increment pointers in tableResults and clauseResults
		while (tableResultsIndex < iTable.resultsTable.size() && clauseResultsIndex < clauseResults.results.size()) {

			int tableValue = iTable.resultsTable[tableResultsIndex][paramIndex];
			int clauseValue = clauseResults.results[clauseResultsIndex][0];
			if (tableValue == clauseValue) { // Same value

				// Get all vectors in iTable overlapping with clauseResult value
				while (tableValue == clauseValue && tableResultsIndex < iTable.resultsTable.size() - 1) {
					mergedResults.push_back(iTable.resultsTable[tableResultsIndex]);
					tableResultsIndex++;
					tableValue = iTable.resultsTable[tableResultsIndex][paramIndex];
				}
				clauseResultsIndex++;

			} else if (tableValue > clauseValue) {
				clauseResultsIndex++;
			} else {
				tableResultsIndex++;
			}
		}

		iTable.setResultsTable(mergedResults);
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
	if (results.size() < 2) { // Single element do nothing
		return results;
	} else {
		int resultsSize = results.size();
		vector<vector<int>> leftArr;
		vector<vector<int>> rightArr;
		copy(results.begin(), results.begin() + (resultsSize / 2), back_inserter(leftArr)); // Copy first to middle
		copy(results.begin() + (resultsSize / 2), results.end(), back_inserter(rightArr)); // Copy middle + 1 to last
		leftArr = mergeSortResults(index, leftArr);
		rightArr = mergeSortResults(index, rightArr);

		vector<vector<int>> mergedResults;
		int leftIndex = 0;
		int rightIndex = 0;
		while (leftIndex < leftArr.size() || rightIndex < rightArr.size()) {
			if (leftIndex >= leftArr.size()) { // Left array exhausted
				mergedResults.push_back(rightArr[rightIndex]);
				rightIndex++; 
				continue;
			}
			if (rightIndex >= rightArr.size()) { // Right array exhausted
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
		if (iTable1.getParamIndex(paramInt.first) != -1) return iTable1;
	}

	IntermediateTable mergedTable;
	mergedTable.instantiateTable();

	// Add table params
	for (size_t i = 0; i < iTable1.tableParams.size(); i++) {
		mergedTable.addTableParams(iTable1.getParamFromIndex(i));
	}
	for (size_t j = 0; j < iTable2.tableParams.size(); j++) {
		mergedTable.addTableParams(iTable2.getParamFromIndex(j));
	}

	// Cross product of two results tables
	vector<vector<int>> mergedResultsTable;
	for (vector<int> table1Row : iTable1.resultsTable) {
		if (!iTable2.tableHasResults()) { // Add vector in if table2 empty
			mergedResultsTable.push_back(table1Row);
		} else {
			for (vector<int> table2Row : iTable2.resultsTable) { // Concat vectors and add
				vector<int> mergedRow = table1Row;
				for (int table2RowValue : table2Row) {
					mergedRow.push_back(table2RowValue);
				}
				mergedResultsTable.push_back(mergedRow);
			}
		}
	}

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
IntermediateTable EvaluatorHelper::findTableWithParam(Param p, vector<IntermediateTable> &iTables) {

	for (IntermediateTable iTable : iTables) {
		int paramIndex = iTable.getParamIndex(p);
		if (paramIndex > -1) {
			return iTable;
		}
	}

	IntermediateTable emptyTable;
	emptyTable.instantiateTable(); 
	emptyTable.hasResults = true; // Assume has results
	return emptyTable;
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

