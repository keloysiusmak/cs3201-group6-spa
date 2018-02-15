#include "Evaluator.h"

using namespace std;

string FOLLOWS = "follows";
string FOLLOWSTAR = "followstar";

Evaluator::Evaluator(QueryObject queryObj) {
	queryObject = queryObj;
};

list<string> Evaluator::evaluateQuery() {
	for (Clause clause : queryObject.getClauses()) {
		evaluateClause(clause);
	}
	return{ "Evaluating query " };
};

ClauseResults Evaluator::evaluateClause(Clause clause) {
	ClauseResults clauseResults = ClauseResults();
	return clauseResults;
};

ClauseResults Evaluator::evaluateFollows() {
	ClauseResults FollowsResults = ClauseResults();
	return FollowsResults;
};

ClauseResults Evaluator::evaluateFollowsStar() {
	ClauseResults followStarResults = ClauseResults();
	return followStarResults;
};
