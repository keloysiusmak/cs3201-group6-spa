#include "Evaluator.h"

using namespace std;

string FOLLOWS = "follows";
string FOLLOWSTAR = "followstar";

string STMT_SYN = "stmtSyn";
string STMT_NUM = "stmtNumber";

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

	ClauseResults FollowsResults = ClauseResults(clause);
	Param firstParam = clause.getFirstParam();
	Param secondParam = clause.getSecondParam();

	if (clause.getFirstParam().type == STMT_SYN) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//getAllFollows()
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//getFollowsBefore(secondParam.value)
		} else {}
	}
	else if (clause.getFirstParam().type == STMT_NUM) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//getFollowsAfter(firstParam.value)
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//checkFollows(firstParam.value, secondParam.value)
		} else {}
	} else {}

	return FollowsResults;
};

ClauseResults Evaluator::evaluateFollowStar(Clause clause) {
	ClauseResults followStarResults = ClauseResults(clause);
	if (clause.getFirstParam().type == STMT_SYN) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//getAllFollowsStar()
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//getFollowsStarBefore(secondParam.value)
		} else {}
	}
	else if (clause.getFirstParam().type == STMT_NUM) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//getFollowsStarAfter(firstParam.value)
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//checkFollowsStar(firstParam.value, secondParam.value)
		} else {}
	} else {}
	return followStarResults;
};
