#include "EvaluatorHelper.h"

using namespace std;

void EvaluatorHelper::consolidateClauses(vector<Clause> const &clauses, vector<Clause> &consolidated) {
	for (Clause clause : clauses) { consolidated.push_back(clause); } }
void EvaluatorHelper::consolidateClauses(vector<Pattern> const &clauses, vector<Clause> &consolidated) {
	for (Clause clause : clauses) { consolidated.push_back(clause); } }

// Preliminary sorting by synonyms 
vector<vector<Clause>> EvaluatorHelper::sortIntoGroups(vector<Clause> &clauses) {
	map<Param, int> paramsHash = ufdsParams(clauses);
};

map<Param, int> EvaluatorHelper::ufdsParams(vector<Clause> &clauses) {
	// Hash all params
	map<Param, Node> paramsHash;
	for (Clause clause : clauses) {
		Param lhs = clause.getLeftParam();
		Param rhs = clause.getRightParam();
		if (Utils::isSynonym(lhs) && lhs.type != ALL) {
			Node newNode = Node(lhs);
			paramsHash[lhs] = newNode;
		}
		if (Utils::isSynonym(rhs) && lhs.type != ALL) {
			Node newNode = Node(rhs);
			paramsHash[lhs] = newNode;
		}
		if (clause.getRelRef() == None) { // Check for pattern
			Pattern* p = reinterpret_cast<Pattern*>(&clause);
			Param patternEnt = p->getEntity();
			Node newNode = Node(patternEnt);
			paramsHash[patternEnt] = newNode;
		} else { ; }
	}

	// Group according to clauses
	for (Clause clause : clauses) {
		Param lhs = clause.getLeftParam();
		Param rhs = clause.getRightParam();
		if (Utils::isSynonym(lhs) && lhs.type != ALL &&
			Utils::isSynonym(rhs) && rhs.type != ALL) {
			unionParams(lhs, rhs, paramsHash);
		}
	}

	//for (pair<Param, Node> paramNode : paramsHash) {
	//	
	//}

	map<Param, int> paramGroups;
	return paramGroups;

};

// Unions two given params
void EvaluatorHelper::unionParams(Param p1, Param p2, map<Param, Node> &paramsHash) {
	Node p1Node = paramsHash[p1];
	Node p2Node = paramsHash[p2];
	
	Node p1NodeSet = findSet(p1Node, paramsHash);
	Node p2NodeSet = findSet(p2Node, paramsHash);

	if (!Utils::isSameParam(p1NodeSet.self, p2NodeSet.self)) {
		p1NodeSet.parent = &p2NodeSet;
	}
};

// Finds the Set of the given node
Node EvaluatorHelper::findSet(Node &n, map<Param, Node> &paramsHash) {
	if (!n.hasParent) return n;
	Node parent = findSet(*n.parent, paramsHash);
	return *n.parent;
}

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

/* Returns number of params of With Clause in table */
int EvaluatorHelper::withClauseNumSyns(Clause &clause, IntermediateTable &iTable) {
	int numSyns = 0;
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();
	if (iTable.getParamIndex(lhs) > -1) numSyns++; // Assume syn param in table
	if (iTable.getParamIndex(rhs) > -1) numSyns++; // Assume syn param in table
	return numSyns;
};

