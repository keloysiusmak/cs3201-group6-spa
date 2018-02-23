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
		if (queryObject.getClauses().size() > 0) {
			cResults.instantiateClause(queryObject.getClauses()[0]);
			evaluateClause(queryObject.getClauses()[0], cResults);
		}
		/* Evaluate pattern and store in pResults obj */
		if (queryObject.getPatterns().size() > 0) {

		}

		list<string> ans = resultToString(cResults, selectParam);
		return ans;
	}
	else {
		list<string> invalidQuery = { invalidQueryMessage };
		return invalidQuery;
	}
};

void Evaluator::evaluateClause(Clause &clause, ClauseResults &clauseResults) {
	RelRef relation = clause.getRelRef();
	if (relation == Follows) {
		evaluateFollows(clause, clauseResults);
	}
	else if (relation == FollowsT) {
		evaluateFollowStar(clause, clauseResults);
	}
	else if (relation == Parent) {
		evaluateParent(clause, clauseResults);
	}
	else if (relation == ParentT) {
		evaluateParentStar(clause, clauseResults);
	}
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
	//	//evaluateModifies(clause, clauseResults); else {}
};

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

void Evaluator::evaluateParent(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) { // (syn, syn)
		if (Utils::isSynonym(rightParam.type)) {
			vector<vector<int>> results = pkb.getAllParent();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			int result = pkb.getParent(stoi(rightParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setValues(vectorResult);
			intersectSingle(clauseResults);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> result = pkb.getChildren(stoi(leftParam.value));
			clauseResults.setValues(result);
			intersectSingle(clauseResults);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkParent(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};

void Evaluator::evaluateParentStar(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			unordered_map<int, vector<int>> results = pkb.getAllParentStar();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			vector<int> results = pkb.getParentStar(stoi(rightParam.value));
			clauseResults.setValues(results);
			intersectSingle(clauseResults);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> results = pkb.getChildrenStar(stoi(leftParam.value));
			clauseResults.setValues(results);
			intersectSingle(clauseResults);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkParentStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};

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

void Evaluator::evaluatePattern(PatternObject &pattern, PatternResults &patternResults) {

	//int leftParam = pattern.LHSid();
	//int rightParam = pattern.RHSid();

	//if (Utils::isSynonym(leftParam)) { // (syn, smth)
		//if (pattern.RHS_type == 0) { // pattern a(syn, variable)
			//vector<int> vectorResult = pkb.getStatementsWithVariable(pattern.RHS);
			//patternResults.setValues(vectorResult);
			//intersectSingle(patternResults);
		//}
		//else if (pattern.RHS_type == 1) { // (syn, concrete)
			//vector<int> vectorResult = pkb.getAllStatementModifiesVariables(pattern.RHS);
			//patternResults.setValues(vectorResult);
			//intersectSingle(patternResults);
		//}
		//else if (pattern.RHS_type == 2) {
			//vector<int> result = pkb.getStatementsWithConstants(pattern.RHS); //intersect with assignment statements
			//patternResults.setValues(result);
			//intersectSingle(patternResults);
		//}
	//}
	//else if (pattern.getLHStype == 0) { // first param is variable
		//if (pattern.RHS_type == 0) { // (concrete, syn)
			//bool result = pkb.checkVariableModifiesVariable(stoi(pattern.LHS.value), stoi(pattern.RHS.value));
			//patternResults.setValues(result);
		//}
		//else if (pattern.RHS_type == 1) { // (concrete, conrete)
			//vector<int> result = pkb.getStatementsFromUsesVariables(stoi(pattern.RHS.value));
			//patternResults.setValues(result);
			//intersectSingle(patternResults);
		//
		//else if (pattern.RHS_type == 2) {
			//vector<int> result = pkb.getStatementsWithConstants(pattern.RHS); //intersect with statements modifies LHS
			//patternResults.setValid(result);
			//intersectSingle(patternResults); //but supposed to return boolean
		//}
	//}
	//else { //first param is blank
		//if (pattern.RHS_type == 0) {
			////(_,_"VARIABLE"_)
			//vector<int> result = pkb.getStatementsWithVariable(pattern.RHS);
			//patternResults.setValues(result);
			//intersectSingle(patternResults);
		//}
		//else if (pattern.RHS_type == 1) {
			////(_,_);
			//vector<int> result = pkb.getAllStatementsWithType(1);
			//patternResults.setValues(result);
			//intersectDouble(patternResults);
		//}
		//else if (pattern.RHS_type == 2) {
			////(_,_"CONSTANT")
			//vector<int> result = pkb.getStatementsWithConstant(stoi(rightParam.value));
			//patternResults.setValues(result);
			//intersectSingle(patternResults);
		//}
	//}
};

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
