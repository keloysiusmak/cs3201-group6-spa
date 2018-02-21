#include "Evaluator.h"

using namespace std;

string FOLLOWS = "follows";
string FOLLOWSTAR = "followstar";
string PARENT = "parent";
string PARENTSTAR = "parentstar";

string STMT_SYN = "stmtSyn";
string STMT_NUM = "stmtNumber";

Evaluator::Evaluator() {
};

Evaluator::Evaluator(QueryObject queryObj) {
	queryObject = queryObj;
};

void Evaluator::setPKB(PKB generatedPKB) {
	pkb = generatedPKB;
};

void Evaluator::setQueryObject(QueryObject queryObj) {
	queryObject = queryObj;
};

list<string> Evaluator::evaluateQuery() {
	Param selectParam = queryObject.getSelectStatement();
	ClauseResults cResults = ClauseResults();
	PatternResults pResults = PatternResults();
	if (queryObject.getClauses().size() > 0) {
		cResults.instantiateClause(queryObject.getClauses()[0]);
	}
	if (queryObject.getPatterns().size() > 0) {

	}
	return{ "evaluating query " };
};

void Evaluator::evaluateClause(Clause &clause, ClauseResults &clauseResults) {
	string relation = clause.getRelRef();
	if (relation == FOLLOWS) {
		evaluateFollows(clause, clauseResults);
	} else if (relation == FOLLOWSTAR) {
		evaluateFollowStar(clause, clauseResults);
	}
	else if (relation == PARENT) {
		evaluateParent(clause, clauseResults);
	}
	else if (relation == PARENTSTAR) {
		evaluateParentStar(clause, clauseResults);
	} else {}
};

void Evaluator::evaluateFollows(Clause &clause, ClauseResults &clauseResults) {

	Param firstParam = clause.getFirstParam();
	Param secondParam = clause.getSecondParam();

	if (firstParam.type == STMT) {
		if (secondParam.type == STMT) { // Return combination of all results
			vector<vector<int>> results = pkb.getAllFollows();
		}
		else if (secondParam.type == PROG_LINE) { // Return prog_line directly before second param
			int result = pkb.getFollowsBefore(stoi(secondParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setKeys(vectorResult);
		}
		else { ; }
	}
	else if (firstParam.type == PROG_LINE) {
		if (secondParam.type == STMT) { // Return prog_line directly after first param
			int result = pkb.getFollowsAfter(stoi(firstParam.value));
			vector<int> vectorResult = { result };
			clauseResults.setKeys(vectorResult);
		}
		else if (secondParam.type == PROG_LINE) { // Boolean value for checking
			bool result = pkb.checkFollows(stoi(firstParam.value), stoi(secondParam.value));
			clauseResults.setValid(result);
		}
		else { ; }
	}
	else { ; }
};

void Evaluator::evaluateFollowStar(Clause &clause, ClauseResults &clauseResults) {

	Param firstParam = clause.getFirstParam();
	Param secondParam = clause.getSecondParam();

	if (firstParam.type == STMT) {
		if (secondParam.type == STMT) {
			unordered_map<int, vector<int>> results = pkb.getAllFollowsStar();
			storeMapToResults(clauseResults, results);
		}
		else if (secondParam.type == PROG_LINE) {
			vector<int> results = pkb.getFollowsBeforeStar(stoi(secondParam.value));
		}
		else { ; }
	}
	else if (firstParam.type == PROG_LINE) {
		if (secondParam.type == STMT) {
			vector<int> results = pkb.getFollowsAfterStar(stoi(firstParam.value));
		}
		else if (secondParam.type == PROG_LINE) {
			bool result = pkb.checkFollowsStar(stoi(firstParam.value), stoi(secondParam.value));
		}
		else { ; }
	}
	else { ; }
};

//ClauseResults Evaluator::evaluateParent(Clause clause) {
//
//	ClauseResults ParentResults = ClauseResults(clause);
//	Param firstParam = clause.getFirstParam();
//	Param secondParam = clause.getSecondParam();
//
//	if (clause.getFirstParam().type == STMT_SYN) {
//		if (clause.getSecondParam().type == STMT_SYN) {
//			//Parent::getAllParents() if select firstParam
//			//Parent::getAllChildren() if select secondParam
//		}
//		else if (clause.getSecondParam().type == STMT_NUM) {
//			//Parent::getParent(clause.getSecondParam());
//		}
//		else {}
//	}
//	else if (clause.getFirstParam().type == STMT_NUM) {
//		if (clause.getSecondParam().type == STMT_SYN) {
//			//Parent::getChildren(clause.getFirstParam());
//		}
//		else if (clause.getSecondParam().type == STMT_NUM) {
//			//Parentt::isParent(clause.getFirstParam(), clause.getSecondParam());
//		}
//		else {}
//	}
//	else {}
//
//	return ParentResults;
//};
//
//ClauseResults Evaluator::evaluateParentStar(Clause clause) {
//	ClauseResults ParentStarResults = ClauseResults(clause);
//	if (clause.getFirstParam().type == STMT_SYN) {
//		if (clause.getSecondParam().type == STMT_SYN) {
//			//Parent::getAllParentsStar() if select firstParam
//			//Parent::getAllChildrenStar() if select secondParam
//		}
//		else if (clause.getSecondParam().type == STMT_NUM) {
//			//Parent::getParentStar(clause.getSecondParam());
//		}
//		else {}
//	}
//	else if (clause.getFirstParam().type == STMT_NUM) {
//		if (clause.getSecondParam().type == STMT_SYN) {
//			//Parent::getChildrenStar(clause.getFirstParam());
//		}
//		else if (clause.getSecondParam().type == STMT_NUM) {
//			//Parentt::isParentStar(clause.getFirstParam(), clause.getSecondParam());
//		}
//		else {}
//	}
//	else {}
//	return ParentStarResults;
//};

/* Iterates through key value pair in unorderedMap and stores the corresponding rows */
void Evaluator::storeMapToResults(ClauseResults &clauseResults, unordered_map<int, vector<int>> map) {
	vector<int> keys;
	vector<vector<int>> values;
	for (auto row : map) {
		keys.push_back(row.first);
		values.push_back(row.second);
	}
	clauseResults.setKeys(keys);
	clauseResults.setValues(values);
};

