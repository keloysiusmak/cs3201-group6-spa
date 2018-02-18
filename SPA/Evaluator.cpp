#include "Evaluator.h"

using namespace std;

string FOLLOWS = "follows";
string FOLLOWSTAR = "followstar";
string PARENT = "parent";
string PARENTSTAR = "parentstar";

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
	ClauseResults clauseResults = ClauseResults(clause);
	string relation = clause.getRelRef();
	if (relation == FOLLOWS) {
		clauseResults = evaluateFollows(clause);
	} else if (relation == FOLLOWSTAR) {
		clauseResults = evaluateFollowStar(clause);
	}
	else if (relation == PARENT) {
		clauseResults = evaluateParent(clause);
	}
	else if (relation == PARENTSTAR) {
		clauseResults = evaluateParentStar(clause);
	} else {}

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


ClauseResults Evaluator::evaluateParent(Clause clause) {

	ClauseResults ParentResults = ClauseResults(clause);
	Param firstParam = clause.getFirstParam();
	Param secondParam = clause.getSecondParam();

	if (clause.getFirstParam().type == STMT_SYN) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//Parent::getAllParents() if select firstParam
			//Parent::getAllChildren() if select secondParam
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//Parent::getParent(clause.getSecondParam());
		}
		else {}
	}
	else if (clause.getFirstParam().type == STMT_NUM) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//Parent::getChildren(clause.getFirstParam());
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//Parentt::isParent(clause.getFirstParam(), clause.getSecondParam());
		}
		else {}
	}
	else {}

	return ParentResults;
};

ClauseResults Evaluator::evaluateParentStar(Clause clause) {
	ClauseResults ParentStarResults = ClauseResults(clause);
	if (clause.getFirstParam().type == STMT_SYN) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//Parent::getAllParentsStar() if select firstParam
			//Parent::getAllChildrenStar() if select secondParam
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//Parent::getParentStar(clause.getSecondParam());
		}
		else {}
	}
	else if (clause.getFirstParam().type == STMT_NUM) {
		if (clause.getSecondParam().type == STMT_SYN) {
			//Parent::getChildrenStar(clause.getFirstParam());
		}
		else if (clause.getSecondParam().type == STMT_NUM) {
			//Parentt::isParentStar(clause.getFirstParam(), clause.getSecondParam());
		}
		else {}
	}
	else {}
	return ParentStarResults;
};

