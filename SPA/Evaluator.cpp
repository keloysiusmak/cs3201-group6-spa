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
	string relation = clause.getRelRef();
	if (relation == FOLLOWS) {
		clauseResults = evaluateFollows(clause);
	} else if (relation == FOLLOWSTAR) {
		clauseResults = evaluateFollowStar(clause);
	}
	else {}

	return clauseResults;
};

ClauseResults Evaluator::evaluateFollows(Clause clause) {
	ClauseResults FollowsResults = ClauseResults();
	return FollowsResults;
};

ClauseResults Evaluator::evaluateFollowStar(Clause clause) {
	ClauseResults followStarResults = ClauseResults();
	return followStarResults;
};
