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

QueryEvaluator::QueryEvaluator()
{
}

QueryEvaluator::QueryEvaluator(QueryObject)
{
}

list<string> QueryEvaluator::evaluateQuery()
{
	return list<string>();
}

bool QueryEvaluator::isValidQuery()
{
	return false;
}

QueryObject QueryEvaluator::getQueryObject()
{
	return QueryObject();
}

void QueryEvaluator::setQueryObject(QueryObject)
{
}

void QueryEvaluator::setInvalidQuery(string)
{
}

void QueryEvaluator::setPKB(PKB)
{
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

bool QueryEvaluator::queryHasClause(QueryObject & queryObj)
{
	return false;
}

bool QueryEvaluator::queryHasPattern(QueryObject & queryObj)
{
	return false;
}

bool QueryEvaluator::hasClauseResults(ClauseResults & clauseResults)
{
	return false;
}
