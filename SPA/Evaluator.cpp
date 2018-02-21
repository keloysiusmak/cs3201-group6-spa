#include "Evaluator.h"
#include <set> 

using namespace std;

string FOLLOWS = "follows";
string FOLLOWSTAR = "followstar";
string PARENT = "parent";
string PARENTSTAR = "parentstar";

string STMT_SYN = "stmtSyn";
string STMT_NUM = "stmtNumber";

Evaluator::Evaluator() {
};

Evaluator::Evaluator(QueryObject queryObj) {
	queryObject = queryObj;
};

void Evaluator::setPKB(PKB generatedPKB) {
	pkb = generatedPKB;
};

void Evaluator::setQueryObject(QueryObject queryObj) {
	queryObject = queryObj;
};

list<string> Evaluator::evaluateQuery() {
	Param selectParam = queryObject.getSelectStatement();
	ClauseResults cResults = ClauseResults();
	PatternResults pResults = PatternResults();
	if (queryObject.getClauses().size() > 0) {
		cResults.instantiateClause(queryObject.getClauses()[0]);
	}
	if (queryObject.getPatterns().size() > 0) {

	}
	list<string> ans = resultToString(cResults, selectParam);
	return ans;
};

void Evaluator::evaluateClause(Clause &clause, ClauseResults &clauseResults) {
	string relation = clause.getRelRef();
	if (relation == FOLLOWS) {
		evaluateFollows(clause, clauseResults);
	} else if (relation == FOLLOWSTAR) {
		evaluateFollowStar(clause, clauseResults);
	}
	else if (relation == PARENT) {
		evaluateParent(clause, clauseResults);
	}
	else if (relation == PARENTSTAR) {
		evaluateParentStar(clause, clauseResults);
	} else {}
};

list<string> Evaluator::resultToString(ClauseResults &clauseResults, Param &selected) {
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;
	set<int> answerSet;
	// Retrieve values from nested vectors of values
	if (selected.value == rightParam.value) {
		for (auto row : clauseResults.values) {
			for (auto rowValue : row) {
				answerSet.insert(rowValue);
			}
		}
	}
	// Print keys directly
	else {
		for (auto key : clauseResults.keys) {
			answerSet.insert(key);
		}
	}

	// Store set of answers into list of strings
	list<string> stringAns;
	for (auto key : answerSet) {
		stringAns.push_back(to_string(key));
	}
	return stringAns;
};

void Evaluator::evaluateFollows(Clause &clause, ClauseResults &clauseResults) {

	Param firstParam = clause.getFirstParam();
	Param secondParam = clause.getSecondParam();

	if (firstParam.type == STMT) {
		if (secondParam.type == STMT) { // Return combination of all results
			vector<vector<int>> results = pkb.getAllFollows();
		}
		else if (secondParam.type == PROG_LINE) { // Return prog_line directly before second param
			int result = pkb.getFollowsBefore(stoi(secondParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setKeys(vectorResult);
		}
		else { ; }
	}
	else if (firstParam.type == PROG_LINE) {
		if (secondParam.type == STMT) { // Return prog_line directly after first param
			int result = pkb.getFollowsAfter(stoi(firstParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setKeys(vectorResult);
		}
		else if (secondParam.type == PROG_LINE) { // Boolean value for checking
			bool result = pkb.checkFollows(stoi(firstParam.value), stoi(secondParam.value));
			clauseResults.setValid(result);
		}
		else { ; }
	}
	else { ; }
};

void Evaluator::evaluateFollowStar(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (leftParam.type == STMT) {
		if (rightParam.type == STMT) {
			unordered_map<int, vector<int>> results = pkb.getAllFollowsStar();
			storeMapToResults(clauseResults, results);
		}
		else if (rightParam.type == PROG_LINE) {
			vector<int> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
		}
		else { ; }
	}
	else if (leftParam.type == PROG_LINE) {
		if (rightParam.type == STMT) {
			vector<int> results = pkb.getFollowsAfterStar(stoi(leftParam.value));
		}
		else if (rightParam.type == PROG_LINE) {
			bool result = pkb.checkFollowsStar(stoi(leftParam.value), stoi(rightParam.value));
		}
		else { ; }
	}
	else { ; }
};

/* Iterates through key value pair in unorderedMap and stores the corresponding rows */
void Evaluator::storeMapToResults(ClauseResults &clauseResults, unordered_map<int, vector<int>> map) {
	vector<int> keys;
	vector<vector<int>> values;
	for (auto row : map) {
		keys.push_back(row.first);
		values.push_back(row.second);
	}
	clauseResults.setKeys(keys);
	clauseResults.setValues(values);
};

