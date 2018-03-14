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

vector<vector<int>> QueryEvaluator::evaluateClause(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateFollows(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateFollowStar(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateParent(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateParentStar(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateUses(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateModifies(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateNext(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateNextStar(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateCalls(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluateCallsStar(Clause & clause, ClauseResults & clauseResults)
{
	return vector<vector<int>>();
}

vector<vector<int>> QueryEvaluator::evaluatePattern(Pattern & pattern, ClauseResults & patternResults)
{
	return vector<vector<int>>();
}

bool QueryEvaluator::hasClauseResults(ClauseResults & clauseResults)
{
	if (clauseResults.valid || clauseResults.values.size() || clauseResults.keyValues.size()) return true;
	return false;
};
