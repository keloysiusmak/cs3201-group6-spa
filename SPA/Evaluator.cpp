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
};

/* getters and setter Methods */
void Evaluator::setPKB(PKB generatedPKB) {
	pkb = generatedPKB;
};

void Evaluator::setQueryObject(QueryObject queryObj) {
	validQuery = true;
	queryObject = queryObj;
};

QueryObject Evaluator::getQueryObject() {
	return queryObject;
};

/* Query Methods */
bool Evaluator::isValidQuery() {
	return validQuery;
};

void Evaluator::setInvalidQuery(string message) {
	validQuery = false;
	invalidQueryMessage = message;
};

/* Main evaluator */
list<string> Evaluator::evaluateQuery() {
	if (isValidQuery()) {
		Param selectParam = queryObject.getSelectStatement();
		Clause clause; Pattern pattern;
		ClauseResults cResults = ClauseResults();
		ClauseResults pResults = ClauseResults();

		if (queryHasClause(queryObject)) {
			clause = queryObject.getClauses()[0];
			evaluateClause(clause, cResults);
		}
		if (queryHasPattern(queryObject)) {
			pattern = queryObject.getPatterns()[0];
			evaluatePattern(pattern, pResults);
		}

		/* Check if param in clause or pattern */
		if (!selectParamInClauses(queryObject)) { // Param independent of clauses
			if (hasClauseResults(cResults) || hasClauseResults(pResults)) { // Results exists for either
				return getAllSelectedParam(selectParam);
			}
			else if (!queryHasClause(queryObject) && !queryHasPattern(queryObject)) { // No clauses
				return getAllSelectedParam(selectParam);
			}
			else {
				return{};
			}
		}
		else {
			if (queryHasClause(queryObject) && queryHasPattern(queryObject)) { // Both Clause and Pattern exist
				list<string> clauseResults = resultToStringList(cResults, selectParam);
				list<string> patternResults = resultToStringList(pResults, selectParam);

				if (selectParamInClause(selectParam, clause) && selectParamInPattern(selectParam, pattern)) { // Param exists in both
					list<string> intersectedAns;
					for (string clauseAns : clauseResults) {
						for (string patternAns : patternResults) {
							if (clauseAns == patternAns) intersectedAns.push_back(clauseAns);
						}
					}
					return intersectedAns;
				}
				else if (selectParamInClause(selectParam, clause)) { // Param exists only in clause
					if (hasClauseResults(pResults)) return clauseResults;
					else return{};
				}
				else { // Param exists only in pattern
					if (hasClauseResults(cResults)) return clauseResults;
					else return{};
				}
			}
			else if (queryHasClause(queryObject)) { // Only Clause exists
				return resultToStringList(cResults, selectParam);
			}
			else { // Only Pattern exists
				return resultToStringList(pResults, selectParam);
			}
		}
	}
	else { // Invalid query
		return{};
	}
};

/* Check whether selected synonym is used in clauses */
bool Evaluator::selectParamInClauses(QueryObject &queryObj) {
	Param selectParam = queryObj.getSelectStatement();
	vector<Clause> clauses = queryObj.getClauses();
	vector<Pattern> patterns = queryObj.getPatterns();
	if (clauses.size() > 0) {
		if (selectParamInClause(selectParam, clauses[0])) return true;
	}
	if (patterns.size() > 0) {
		if (selectParamInPattern(selectParam, patterns[0])) return true;
	}
	return false;
};

bool Evaluator::selectParamInClause(Param select, Clause &clause) {
	if (Utils::isSameParam(clause.getFirstParam(), select)) return true;
	if (Utils::isSameParam(clause.getSecondParam(), select)) return true;
	return false;
}

bool Evaluator::selectParamInPattern(Param select, Pattern &pattern) {
	if (Utils::isSameParam(pattern.getLeftParam(), select)) return true;
	if (Utils::isSameParam(pattern.getRightParam(), select)) return true;
	if (Utils::isSameParam(pattern.getEntity(), select)) return true;
	return false;
}

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

/* Get all selected params when Select param not in clause/pattern */
list<string> Evaluator::getAllSelectedParam(Param p) {
	int paramIntType = statementTypeToIntMap(p.type);
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
};

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
	else if (relation == UsesS) {
		evaluateUses(clause, clauseResults);
	}
	else if (relation == ModifiesS) {
		evaluateModifies(clause, clauseResults);
	}
	else { ; } // For extension
};

/* Assumes Selected is within ClauseResults */
list<string> Evaluator::resultToStringList(ClauseResults &clauseResults, Param &selected) {
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;
	set<int> answerSet;

	if (Utils::isSameParam(selected, clauseResults.entRef)) { // Get selected from pattern assignments
		for (int value : clauseResults.assignmentsEnts) {
			answerSet.insert(value);
		}
	}
	else if (clauseResults.values.size()) { // Get selected from values 
		if (clauseResults.values[0] == 0) return {};
		for (int value : clauseResults.values) {
			answerSet.insert(value);
		}
	}
	else if (clauseResults.keyValues.size()) { // Get selected from hashtable
		for (auto pair : clauseResults.keyValues) {
			if (Utils::isSameParam(selected, leftParam)) {
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
		return{};
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

			/* Consolidate key parent and value vector of children */
			unordered_map<int, vector<int>> parentResults;
			for (auto pair : results) {
				auto it = parentResults.find(pair[0]);
				if (it != parentResults.end()) {
					vector<int> *currentChildren = &(it->second);
					currentChildren->push_back(pair[1]);
				}
				else {
					vector<int> children = { pair[1] };
					parentResults.insert({ pair[0], children });
				}
			}

			clauseResults.setkeyValues(parentResults);
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
				intersectSingle(clauseResults);
			}
			else { // LHS is var_name
				int variableId = pkb.getVariableId(rightParam.value);
				vector<int> results = pkb.getStatementsFromUsesVariable(variableId);
				clauseResults.setValues(results);
				intersectSingle(clauseResults);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<int> results = pkb.getUsesVariablesFromStatement(stoi(leftParam.value));
			clauseResults.setValues(results);
		}
		else { // (concrete, concrete)
			bool result;
			if (rightParam.type == IDENT) {
				int varId = pkb.getVariableId(rightParam.value);
				result = pkb.checkStatementUsesVariable(stoi(leftParam.value), varId);
			}
			else {
				result = pkb.checkStatementUsesVariable(stoi(leftParam.value), stoi(rightParam.value));
			}
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
			intersectSingle(clauseResults);
		}
		else { // (concrete, concrete)
			bool result;
			if (rightParam.type == IDENT) {
				int varId = pkb.getVariableId(rightParam.value);
				result = pkb.checkStatementModifiesVariable(stoi(leftParam.value), varId);
			}
			else {
				result = pkb.checkStatementModifiesVariable(stoi(leftParam.value), stoi(rightParam.value));
			}
			clauseResults.setValid(result);
		}
	}
};

/* Pattern cases: 
LHS: _, VARIABLE, IDENT
RHS: _, VAR_NAME, CONSTANT
*/
void Evaluator::evaluatePattern(Pattern &pattern, ClauseResults &patternResults) {
	patternResults.instantiatePattern(pattern);

	Param leftParam = pattern.getLeftParam();
	Param rightParam = pattern.getRightParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (v/_, IDENT/CONSTANT)
			unordered_map<int, vector<int>> statementsUsing = pkb.getAllStatementModifiesVariables();
			patternResults.setkeyValues(statementsUsing);

			vector<int> allStatements;
			for (auto keyValuePair : statementsUsing) {
				allStatements.push_back(keyValuePair.first);
			}
			patternResults.setAssignmentsEnts(removeWhileIfs(allStatements)); // Set assignment
		}
		else {
			vector<int> statementsUsing;
			if (rightParam.type == VAR_NAME) { // (v/_, var_name)
				int varId = pkb.getVariableId(rightParam.value);
				statementsUsing = pkb.getStatementsFromUsesVariable(varId);
			}
			else { // (v/_, constant)
				statementsUsing = pkb.getStatementsWithConstant(stoi(rightParam.value));
			}
			/* Get variables */
			vector<int> variables;
			for (int stmt : statementsUsing) {
				vector<int> variableIds = pkb.getModifiesVariablesFromStatement(stmt);
				for (int varId : variableIds) {
					if (find(variables.begin(), variables.end(), varId) == variables.end()) { // If not in vector
						variables.push_back(varId);
					}
					else { ; }
				}
			}
			patternResults.setValues(variables);

			/* Set assignment statements */
			vector<int> assignments = pkb.getAllStatementsWithType(1);
			vector<int> assignmentEnts = intersectVectors(assignments, statementsUsing);
			patternResults.setAssignmentsEnts(assignmentEnts);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (IDENT, var_name/constant) 
			int lhsVarId = pkb.getVariableId(leftParam.value);
			vector<int> statementModifies = pkb.getStatementsFromModifiesVariable(lhsVarId);
			patternResults.setValid(statementModifies.size() > 0);
			patternResults.setAssignmentsEnts(removeWhileIfs(statementModifies)); // Set assignment
		}
		else {
			int lhsVarId = pkb.getVariableId(leftParam.value);
			vector<int> statementModifies = pkb.getStatementsFromModifiesVariable(lhsVarId);
			vector<int> statementUses;
			if (rightParam.type == VAR_NAME) { // (IDENT, var_name)
				int rhsVarId = pkb.getVariableId(rightParam.value);
				statementUses = pkb.getStatementsFromUsesVariable(rhsVarId);
			}
			else { // (IDENT, constant)
				statementUses = pkb.getStatementsWithConstant(stoi(rightParam.value));
			}

			vector<int> assignmentEnts;
			for (int modifiesStmt : statementModifies) { // Check intersection
				for (int UsesStmt : statementUses) {
					if (modifiesStmt == UsesStmt) {
						assignmentEnts.push_back(modifiesStmt);
						patternResults.setValid(true);
					}
				}
			}
			patternResults.setAssignmentsEnts(removeWhileIfs(assignmentEnts));
		}
	}

};

/* Gets all values from left side of unordered map */
vector<int> Evaluator::getAllValuesFromMap(unordered_map<int, vector<int>> map) {
	vector<int> values;
	for (auto keyValuePair : map) {
		for (int value : keyValuePair.second) {
			if (find(values.begin(), values.end(), value) == values.end()) { // If not in vector
				values.push_back(value);
			}
		}
	}
	return values;
}

/* Removes elements in v2 from v1 */
vector<int> Evaluator::removeElems(vector<int> v1, vector<int> v2) {
	vector<int> filtered;
	for (int value : v1) {
		if (find(v2.begin(), v2.end(), value) == v2.end()) { // If not in vector
			filtered.push_back(value);
		}
	}
	return filtered;
	
};

/* Removes all while and if statements from statement list */
vector<int> Evaluator::removeWhileIfs(vector<int> stmts) {
	vector<int> whileStatements = pkb.getAllStatementsWithType(2);
	vector<int> ifStatements = pkb.getAllStatementsWithType(3);
	return removeElems(removeElems(stmts, ifStatements), whileStatements);
};

/* Assumes no duplicates in vectors */
vector<int> Evaluator::intersectVectors(vector<int> &v1, vector<int> &v2) {
	vector<int> filtered;
	for (int v1Value : v1) {
		for (int v2Value : v2) {
			if (v1Value == v2Value) filtered.push_back(v1Value);
		}
	}
	return filtered;
};

void Evaluator::intersectSingle(ClauseResults &clauseResults) {

	int typeInt;
	Param leftParam = clauseResults.lhs;
	Param rightParam = clauseResults.rhs;

	/* Get typeInt for param */
	(Utils::isSynonym(leftParam.type)) ? typeInt = statementTypeToIntMap(leftParam.type) :
		typeInt = statementTypeToIntMap(rightParam.type);

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
	int leftParamIntType = statementTypeToIntMap(leftParam.type);
	int rightParamIntType = statementTypeToIntMap(rightParam.type);
		
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
		unordered_map<int, vector<int>> filteredTable;
		for (auto keyValuePair : keyValueTable) {
			vector<int> filteredStmts = vector<int>();
			for (int validStmt : rightParamValidStmts) {
				for (int stmt : keyValuePair.second) {
					if (validStmt == stmt) {
						filteredStmts.push_back(validStmt);
					}
				}
			}
			if (filteredStmts.size() > 0) filteredTable.insert({keyValuePair.first, filteredStmts});
		}
		clauseResults.setkeyValues(filteredTable);
	}
};

int Evaluator::statementTypeToIntMap(ParamType t) {
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
};
