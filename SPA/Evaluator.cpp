#include "Evaluator.h"
#include <set>

using namespace std;

string VALID_QUERY = "Query is valid";

string STMT_SYN = "stmtSyn";
string STMT_NUM = "stmtNumber";

/* Constructor */
Evaluator::Evaluator() {
	// Defaults true
	validQuery = true;
};

Evaluator::Evaluator(QueryObject queryObj) {
	queryObject = queryObj;
}

/* getters and setter Methods */
void Evaluator::setPKB(PKB generatedPKB) {
	pkb = generatedPKB;
};

void Evaluator::setQueryObject(QueryObject queryObj) {
	queryObject = queryObj;
};

QueryObject Evaluator::getQueryObject() {
	return queryObject;
}

/* Invalid Query Methods */
list<string> Evaluator::invalidQuery(string invalidQueryMessage) {
	return{ invalidQueryMessage };
}

void Evaluator::setInvalidQuery(string message) {
	validQuery = false;
	invalidQueryMessage = message;
};

/* Main evaluator */
list<string> Evaluator::evaluateQuery() {
	if (validQuery) {
		Param selectParam = queryObject.getSelectStatement();
		ClauseResults cResults = ClauseResults();
		PatternResults pResults = PatternResults();

		/* Evaluate clauses and store in cResults obj */
		if (queryObject.getClauses().size() > 0) evaluateClause(queryObject.getClauses()[0], cResults);
		/* Evaluate pattern and store in pResults obj */
		if (queryObject.getPatterns().size() > 0) { ; };

		/* Check if param in clause*/
		if (selectParamInClause(queryObject)) {
				
		}
		else {
			if (hasClauseResults(cResults) || hasPatternResults(pResults)) {
				return getAllSelectedParam(selectParam);
			}
			else {
				return{ "None" };
			}
		}
		list<string> ans = resultToString(cResults, selectParam);
		return ans;
	}
	else {
		list<string> invalidQuery = { invalidQueryMessage };
		return invalidQuery;
	}
};

/* Check whether selected synonym is used in clauses */
bool Evaluator::selectParamInClause(QueryObject &queryObj) {
	string selectParamValue = queryObj.getSelectStatement().value;
	vector<Clause> clauses = queryObj.getClauses();
	vector<Pattern> patterns = queryObj.getPatterns();
	if (clauses.size() > 0) {
		if (clauses[0].getFirstParam().value == selectParamValue) return true;
		if (clauses[0].getSecondParam().value == selectParamValue) return true;
	}
	if (clauses.size() > 0) {
		if (patterns[0].getLeftParam().value == selectParamValue) return true;
		if (patterns[0].getRightParam().value == selectParamValue) return true;
	}
	return false;
};

/* Check if results exist in clause or pattern */
bool Evaluator::hasClauseResults(ClauseResults &clauseResults) {
	if (clauseResults.valid || clauseResults.values.size() || clauseResults.keyValues.size()) return true;
	return false;
};
bool Evaluator::hasPatternResults(PatternResults &patternResults) {
	// To be implemented
	return true;
};

/* Get all selected params */
list<string> Evaluator::getAllSelectedParam(Param p) {
	return{ "To be implemented" };
}

void Evaluator::evaluateClause(Clause &clause, ClauseResults &clauseResults) {
	clauseResults.instantiateClause(queryObject.getClauses()[0]);
	RelRef relation = clause.getRelRef();
	if (relation == Follows) {
		evaluateFollows(clause, clauseResults);
	} else if (relation == FollowsT) {
		evaluateFollowStar(clause, clauseResults);
	//else if (relation == PARENT) {
	//	evaluateParent(clause, clauseResults);
	//}
	//else if (relation == PARENTSTAR) {
	//	evaluateParentStar(clause, clauseResults);
	////} 
	////else if (relation == USES) {
	//	//evaluateUses(clause, clauseResults);
	////}
	////else if (relation == MODIFIES) {
	//	//evaluateModifies(clause, clauseResults);
	} else {}
};

/* Assumes Selected is within ClauseResults */
list<string> Evaluator::resultToString(ClauseResults &clauseResults, Param &selected) {
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;
	set<int> answerSet;
	// Retrieve values from hashmap vectors of values
	
	// Print values directly

	// Store set of answers into list of strings
	list<string> stringAns;
	for (auto key : answerSet) {
		stringAns.push_back(to_string(key));
	}
	return stringAns;
};

/* Evaluation components */

void Evaluator::evaluateFollows(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollows();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			int result = pkb.getFollowsBefore(stoi(rightParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setValues(vectorResult);
			intersectSingle(clauseResults);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			int result = pkb.getFollowsAfter(stoi(leftParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setValues(vectorResult);
			intersectSingle(clauseResults);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkFollows(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};


void Evaluator::evaluateFollowStar(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			unordered_map<int, vector<int>> results = pkb.getAllFollowsStar();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			vector<int> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
			clauseResults.setValues(results);
			intersectSingle(clauseResults);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> results = pkb.getFollowsAfterStar(stoi(leftParam.value));
			clauseResults.setValues(results);
			intersectSingle(clauseResults);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkFollowsStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};

//void Evaluator::evaluateParent(Clause &clause, ClauseResults &clauseResults) {
//
//	Param leftParam = clause.getFirstParam();
//	Param rightParam = clause.getSecondParam();
//
//	if (leftParam.type == STMT) {
//		if (rightParam.type == STMT) {
//			vector<vector<int>> results = pkb.getAllParent();
//		}
//		else if (rightParam.type == INTEGER) {
//			int result = pkb.getParent(stoi(rightParam.value));
//			vector<int> vectorResult = { result };
//			clauseResults.setKeys(vectorResult);
//		}
//		else { ; }
//	}
//	else if (leftParam.type == INTEGER) {
//		if (rightParam.type == STMT) {
//			vector<int> result = pkb.getChildren(stoi(leftParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (rightParam.type == INTEGER) {
//			bool result = pkb.checkParent(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		else { ; }
//	}
//	else { ; }
//};
//
//void Evaluator::evaluateParentStar(Clause &clause, ClauseResults &clauseResults) {
//
//	Param leftParam = clause.getFirstParam();
//	Param rightParam = clause.getSecondParam();
//
//	if (leftParam.type == STMT) {
//		if (rightParam.type == STMT) {
//			unordered_map<int, vector<int>> results = pkb.getAllParentStar();
//			storeMapToResults(clauseResults, results);
//		}
//		else if (rightParam.type == INTEGER) {
//			vector<int> results = pkb.getParentStar(stoi(rightParam.value));
//			clauseResults.setKeys(results);
//		}
//		else { ; }
//	}
//	else if (leftParam.type == INTEGER) {
//		if (rightParam.type == STMT) {
//			vector<int> results = pkb.getChildrenStar(stoi(leftParam.value));
//			clauseResults.setKeys(results);
//		}
//		else if (rightParam.type == INTEGER) {
//			bool result = pkb.checkParentStar(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		else { ; }
//	}
//	else { ; }
//};

//void Evaluator::evaluateUses(Clause &clause, ClauseResults &clauseResults) {
//
//	Param leftParam = clause.getFirstParam();
//	Param rightParam = clause.getSecondParam();
//
//	if (rightParam.type == VARIABLE) {
//		if (leftParam.type == STMT) {
//			vector<int> result = pkb.getStatementsFromUsesVariable(stoi(rightParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROG_LINE) {
//			bool result = pkb.checkStatementUsesVariable(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		/* else if (leftParam.type == PROCEDURE) {
//			bool result = pkb.checkProcedureUsesVariable(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		else if (leftParam.type == PROC_SYN) {
//			vector<int> result = pkb.getProceduresFromUsesVariable(stoi(rightParam.value));
//			clauseResults.setKeys(result);
//		} */
//		else { ; }
//	}
//	/* else if (rightParam.type == VAR_SYN) {
//		if (leftParam.type == STMT) {
//			if (selectParam == leftParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllStatementUsesVariables();
//				storeMapToResults(clauseResults, results);
//			}
//			else if (selectParam == rightParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllVariableUsesStatements();
//				storeMapToResults(clauseResults, results);
//			}
//			else { ; }
//		}
//		else if (leftParam.type == PROG_LINE) {
//			vector<int> result = pkb.getUsesVariablesFromStatement(stoi(leftParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROCEDURE) {
//			vector <int> result = pkb.getProceduresFromUsesVariable(stoi(leftParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROC_SYN) {
//			if (selectParam == leftParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllProcedureUsesVariables();
//				storeMapToResults(clauseResults, results);
//			}
//			else if (selectParam == rightParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllVariableUsesProcedures();
//				storeMapToResults(clauseResults, results);
//			}
//			else { ; }
//		}
//		else { ; }
//	} */
//	else { ; }
//};
//
//void Evaluator::evaluateModifies(Clause &clause, ClauseResults &clauseResults) {
//
//	Param leftParam = clause.getFirstParam();
//	Param rightParam = clause.getSecondParam();
//
//	/* if (rightParam.type == VARIABLE) {
//		if (leftParam.type == STMT) {
//			vector<int> result = pkb.getStatementsFromModifiesVariables(stoi(rightParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROG_LINE) {
//			bool result = pkb.checkStatementModifiesVariable(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		else if (leftParam.type == PROCEDURE) {
//			bool result = pkb.checkProcedureModifiesVariable(stoi(leftParam.value), stoi(rightParam.value));
//			clauseResults.setValid(result);
//		}
//		else if (leftParam.type == PROC_SYN) {
//			vector<int> result = pkb.getProceduresFromModifiesVariables(stoi(rightParam.value));
//			clauseResults.setKeys(result);
//		}
//		else { ; }
//	}
//	else if (rightParam.type == VAR_SYN) {
//		if (leftParam.type == STMT) {
//			if (selectParam == leftParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllStatementModifiesVariables();
//				storeMapToResults(clauseResults, results);
//			}
//			else if (selectParam == rightParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllVariablesModifiesStatements();
//				storeMapToResults(clauseResults, results);
//			}
//			else { ; }
//		}
//		else if (leftParam.type == PROG_LINE) {
//			vector<int> result = pkb.getModifiesVariablesFromStatement(stoi(leftParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROCEDURE) {
//			vector <int> result = pkb.getProceduresFromModifiesVariables(stoi(leftParam.value));
//			clauseResults.setKeys(result);
//		}
//		else if (leftParam.type == PROC_SYN) {
//			if (selectParam == leftParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllProceduresModifiesVariables();
//				storeMapToResults(clauseResults, results);
//			}
//			else if (selectParam == rightParam.type) {
//				unordered_map<int, vector<int>> results = pkb.getAllVariableModifiesProcedure();
//				storeMapToResults(clauseResults, results);
//			}
//			else { ; }
//		}
//		else { ; }
//	}
//	else { ; } */
//};
//

/* Iterates through key value pair in unorderedMap and stores the corresponding rows */
void Evaluator::storeMapToResults(ClauseResults &clauseResults, unordered_map<int, vector<int>> map) {
	vector<int> keys;
	vector<vector<int>> values;
	for (auto row : map) {
		keys.push_back(row.first);
		values.push_back(row.second);
	}
	//clauseResults.setKeys(keys);
	//clauseResults.setValues(values);
};

void Evaluator::intersectSingle(ClauseResults &clauseResults) {

	int typeInt;
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;

	/* Get typeInt for param */
	(Utils::isSynonym(leftParam.type)) ? typeInt = typeToIntMap(leftParam.type) :
		typeInt = typeToIntMap(rightParam.type);

	/* Return out if synonym is statement */
	if (typeInt == 0) return;

	pkb.getAllStatementsWithType(typeInt);
	vector<int> validTypeStmts = pkb.getAllStatementsWithType(typeInt);
	vector<int> filtered;

	/* Intersect vectors */
	for (int stmt1 : clauseResults.values) {
		for (int stmt2 : validTypeStmts) {
			if (stmt1 == stmt2) {
				filtered.push_back(stmt1);
			}
		}
	}
	clauseResults.setValues(filtered);
};

void Evaluator::intersectDouble(ClauseResults &clauseResults) {

	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;
	int leftParamIntType = typeToIntMap(leftParam.type);
	int rightParamIntType = typeToIntMap(rightParam.type);
		
	/* Filter Keys */
	if (leftParamIntType != 0) {
		unordered_map<int, vector<int>> keyValueTable = clauseResults.keyValues;
		unordered_map<int, vector<int>> filteredTable;
		vector<int> leftParamValidStmts = pkb.getAllStatementsWithType(leftParamIntType);
		for (auto keyValuePair : keyValueTable) {
			for (int stmt : leftParamValidStmts) {
				if (stmt == keyValuePair.first) {
					filteredTable.insert(keyValuePair);
				}
			}
		}
		clauseResults.setkeyValues(filteredTable);
	}

	/* Filter values */
	if (rightParamIntType != 0) {
		unordered_map<int, vector<int>> keyValueTable = clauseResults.keyValues;
		vector<int> rightParamValidStmts = pkb.getAllStatementsWithType(rightParamIntType);
		for (auto keyValuePair : keyValueTable) {
			for (int validStmt : rightParamValidStmts) {
				vector<int> filteredStmts;
				for (int stmt : keyValuePair.second) {
					if (validStmt == stmt) {
						filteredStmts.push_back(validStmt);
					}
				}
				keyValuePair.second = filteredStmts;
			}
		}
	}
}

int Evaluator::typeToIntMap(ParamType t) {
	switch (t) {
	case ASSIGN:
		return 1;
	case WHILE:
		return 2;
	default:
		return 0;
	}
}
