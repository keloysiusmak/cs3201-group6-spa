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

/* Query Methods */
bool Evaluator::isValidQuery() {
	return validQuery;
}

list<string> Evaluator::invalidQuery(string invalidQueryMessage) {
	return{ invalidQueryMessage };
}

void Evaluator::setInvalidQuery(string message) {
	validQuery = false;
	invalidQueryMessage = message;
};

/* Main evaluator */
list<string> Evaluator::evaluateQuery() {
	if (isValidQuery()) {
		Param selectParam = queryObject.getSelectStatement();
		ClauseResults cResults = ClauseResults();
		PatternResults pResults = PatternResults();

		/* Evaluate clauses and store in cResults obj */
		if (queryHasClause(queryObject)) evaluateClause(queryObject.getClauses()[0], cResults);
		/* Evaluate pattern and store in pResults obj */
		if (queryHasPattern(queryObject)) { ; };

		/* Check if param in clause */
		if (!selectParamInClause(queryObject)) {
			if (hasClauseResults(cResults) || hasPatternResults(pResults)) {
				return getAllSelectedParam(selectParam);
			}
			else if (!queryHasClause(queryObject) && !queryHasPattern(queryObject)) {
				return getAllSelectedParam(selectParam);
			}
			else {
				return{ "None" };
			}
		}
		else {
			/* Format results to string */
			list<string> ans = resultToString(cResults, selectParam);
			return ans;
		}
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

/* Check if Clauses or Patterns are present */
bool Evaluator::queryHasClause(QueryObject &queryObj) {
	return queryObj.getClauses().size() > 0;
};

bool Evaluator::queryHasPattern(QueryObject &queryObj) {
	return queryObj.getPatterns().size() > 0;
};

/* Check if results exist in clause or pattern */
bool Evaluator::hasClauseResults(ClauseResults &clauseResults) {
	if (clauseResults.valid || clauseResults.values.size() || clauseResults.keyValues.size()) return true;
	return false;
};
bool Evaluator::hasPatternResults(PatternResults &patternResults) {
	// To be implemented
	return false;
};

/* Get all selected params */
list<string> Evaluator::getAllSelectedParam(Param p) {
	int paramIntType = typeToIntMap(p.type);
	vector<int> pkbResults;
	if (paramIntType != 0) {
		pkbResults = pkb.getAllStatementsWithType(paramIntType);
	}
	else if (p.type == STMT) { 
		pkbResults = pkb.getAllStatements();
	}
	else if (p.type == VARIABLE) {
		pkbResults = pkb.getAllVariables();
	}

	list<string> results;
	for (int ans : pkbResults) {
		(p.type == VARIABLE) ? results.push_back(pkb.getVariableName(ans)) :
			results.push_back(to_string(ans));
	}
	return results;
}

void Evaluator::evaluateClause(Clause &clause, ClauseResults &clauseResults) {
	clauseResults.instantiateClause(queryObject.getClauses()[0]);
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
	////else if (relation == USES) {
	//	//evaluateUses(clause, clauseResults);
	////}
	////else if (relation == MODIFIES) {
	//	//evaluateModifies(clause, clauseResults); else {}
};

/* Assumes Selected is within ClauseResults */
list<string> Evaluator::resultToString(ClauseResults &clauseResults, Param &selected) {
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;
	set<int> answerSet;

	if (clauseResults.values.size()) { // Get selected from values 
		for (int value : clauseResults.values) {
			answerSet.insert(value);
		}
	}
	else if (clauseResults.keyValues.size()) { // Get selected from hashtable
		for (auto pair : clauseResults.keyValues) {
			if (leftParam.type == selected.type) {
				answerSet.insert(pair.first);
			}
			else {
				for (int value : pair.second) {
					answerSet.insert(value);
				}
			}
		}
	}
	else {
		return{ "None" };
	}

	// Store set of answers into list of strings
	list<string> stringAns;
	for (auto key : answerSet) {
		(selected.type != VARIABLE) ? stringAns.push_back(to_string(key)):
			stringAns.push_back(pkb.getVariableName(key));
	}
	return stringAns;
};

/* Evaluation components */

/* Follows cases:
lhs: s, a, if, w, int
rhs: s, a, if, w, int
*/
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

/* Parent cases:
lhs: s, a, if, w, int
rhs: s, a, if, w, int
*/
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

/* Uses cases:
lhs: s, w, if, a, int
rhs: v, var_name, constant
*/
void Evaluator::evaluateUses(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			unordered_map<int, vector<int>> results = pkb.getAllStatementUsesVariables();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			if (rightParam.type == INTEGER) { // RHS is integer constant
				vector<int> results = pkb.getStatementsWithConstant(stoi(rightParam.value));
				clauseResults.setValues(results);
			}
			else { // LHS is var_name
				int variableId = pkb.getVariableId(rightParam.value);
				vector<int> results = pkb.getStatementsFromUsesVariable(variableId);
				clauseResults.setValues(results);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> results = pkb.getUsesVariablesFromStatement(stoi(leftParam.value));
			clauseResults.setValues(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkStatementUsesVariable(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};

/* Modifies cases:
LHS: s, a, if, w, int
RHS: v, var_name
*/
void Evaluator::evaluateModifies(Clause &clause, ClauseResults &clauseResults) {

	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			unordered_map<int, vector<int>> results = pkb.getAllStatementModifiesVariables();
			clauseResults.setkeyValues(results);
			intersectDouble(clauseResults);
		}
		else { // (syn, concrete)
			int variableId = pkb.getVariableId(rightParam.value);
			vector<int> results = pkb.getStatementsFromModifiesVariable(variableId);
			clauseResults.setValues(results);
			intersectSingle(clauseResults);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> results = pkb.getModifiesVariablesFromStatement(stoi(leftParam.value));
			clauseResults.setValues(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkStatementModifiesVariable(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
};


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
	
	/* Short circuit for variable */
	if (rightParam.type == VARIABLE) return;

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
	case IF:
		return 3;
	default:
		return 0;
	}
}
