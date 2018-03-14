#include "QueryEvaluator.h"
#include <set>

using namespace std;

string VALID_QUERY = "Query is valid";

string STMT_SYN = "stmtSyn";
string STMT_NUM = "stmtNumber";

/* Constructor */
QueryEvaluator::QueryEvaluator() {
	// Defaults true
	validQuery = true;
};

QueryEvaluator::QueryEvaluator(QueryObject queryObj) {
	queryObject = queryObj;
};

/* getters and setter Methods */
void QueryEvaluator::setPKB(PKB generatedPKB) {
	pkb = generatedPKB;
};

void QueryEvaluator::setQueryObject(QueryObject queryObj) {
	validQuery = true;
	queryObject = queryObj;
};

QueryObject QueryEvaluator::getQueryObject() {
	return queryObject;
};

/* Query Methods */
bool QueryEvaluator::isValidQuery() {
	return validQuery;
};

void QueryEvaluator::setInvalidQuery(string message) {
	validQuery = false;
	invalidQueryMessage = message;
};

vector<vector<int>> QueryEvaluator::evaluateQuery()
{
	if (isValidQuery()) {
		Param selectParam = queryObject.getSelectStatement(); //one param or tuple
		Clause clause;
		Pattern pattern;
		ClauseResults cResults = ClauseResults();
		ClauseResults pResults = ClauseResults();
		vector<vector<int>> result;
		int numClauses = queryNumClauses(queryObject);
		int numPattern = queryNumPattern(queryObject);

		while (numClauses > 0) {
			clause = queryObject.getClauses()[0];
			evaluateClause(clause, cResults);
			numClauses--;
		}

		while (numPattern > 0) {
			pattern = queryObject.getPatterns()[0];
			evaluateClause(clause, pResults);
			numPattern--;
		}
	}
}

/* for any seleced param that is not in any of the clauses, return all instances of that param */
/* uncomment this out later when bugs are fixed*/
/* add to intermediate table later on*/
/* vector<vector<int>> QueryEvaluator::getAllSelectedParam(Param p) {
	int paramIntType = statementTypeToIntMap(p.type);
	vector<int> pkbResults;
	if (paramIntType != 0) {
		pkbResults = pkb.getAllStatementsWithType(paramIntType);
	}
	else if (p.type == STMT || p.type == PROG_LINE) {
		pkbResults = pkb.getAllStatements();
	}
	else if (p.type == VARIABLE) {
		pkbResults = pkb.getAllVariables();
	}

	vector<vector<int>> results;
	for (int ans : pkbResults) {
		(p.type == VARIABLE) ?
			results.push_back(pkb.getVariableName(ans)) :
			results.push_back(to_string(ans));
	}
	return results;
}; */

int QueryEvaluator::queryNumClauses(QueryObject &queryObj) {
	return queryObj.getClauses().size();
};

int QueryEvaluator::queryNumPattern(QueryObject &queryObj) {
	return queryObj.getPatterns().size();
}

void QueryEvaluator::evaluateClause(Clause & clause, ClauseResults & clauseResults)
{
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
	else if (relation == Next) {
		evaluateNext(clause, clauseResults);
	}
	else if (relation == NextT) {
		evaluateNextStar(clause, clauseResults);
	}
	else { ; } //affects
}

void QueryEvaluator::evaluateFollows(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();


	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollows();
			//clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBefore(stoi(rightParam.value));
			//clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfter(stoi(leftParam.value));
			//clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool results = pkb.checkFollows(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(results);
		}
	}
}

void QueryEvaluator::evaluateFollowStar(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollowsStar();
			//clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
			//clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfterStar(stoi(leftParam.value));
			//clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkFollowsStar(stoi(leftParam.value), stoi(rightParam.value));
			//clauseResults.setValid(result);
		}
	}
}

void QueryEvaluator::evaluateParent(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) { // (syn, syn)
		if (Utils::isSynonym(rightParam.type)) {
			vector<vector<int>> results = pkb.getAllParent();
			//clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParent(stoi(rightParam.value));
			//clauseResults.setValues(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildren(stoi(leftParam.value));
			//clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkParent(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

void QueryEvaluator::evaluateParentStar(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllParentStar();
			//clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParentStar(stoi(rightParam.value));
			//clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildrenStar(stoi(leftParam.value));
			//clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkParentStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

void QueryEvaluator::evaluateUses(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllStatementUsesVariables();
			//clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			if (rightParam.type == INTEGER) { // RHS is integer constant
				vector<vector<int>> results = pkb.getStatementsWithConstant(stoi(rightParam.value));
				//clauseResults.setResults(results);
			}
			else { // LHS is var_name
				int variableId = pkb.getVariableId(rightParam.value);
				vector<vector<int>> results = pkb.getStatementsFromUsesVariable(variableId);
				//clauseResults.setResults(results);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getUsesVariablesFromStatement(stoi(leftParam.value));
			//clauseResults.setResults(results);
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
}

void QueryEvaluator::evaluateModifies(Clause & clause, ClauseResults & clauseResults)
{
	
}

void QueryEvaluator::evaluateNext(Clause & clause, ClauseResults & clauseResults)
{
	
}

void QueryEvaluator::evaluateNextStar(Clause & clause, ClauseResults & clauseResults)
{
	
}

void QueryEvaluator::evaluateCalls(Clause & clause, ClauseResults & clauseResults)
{
	
}

void QueryEvaluator::evaluateCallsStar(Clause & clause, ClauseResults & clauseResults)
{
	
}

void QueryEvaluator::evaluatePattern(Pattern & pattern, ClauseResults & patternResults)
{
	
}

bool QueryEvaluator::hasClauseResults(ClauseResults & clauseResults)
{
	if (clauseResults.valid || clauseResults.values.size() || clauseResults.keyValues.size()) return true;
	return false;
};
