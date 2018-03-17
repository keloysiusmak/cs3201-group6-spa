#include "QueryEvaluator.h"
#include <set>

using namespace std;

string VALID_QUERY = "Query is valid";

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

void QueryEvaluator::setInvalidQuery(list<string> message) {
	validQuery = false;
	invalidQueryMessage = message;
};

/*
  Possible Optimizations: Check if clauseresults or table is empty at any point of time, if yes, return null
*/
list<string> QueryEvaluator::evaluateQuery() {
	if (isValidQuery()) {

		vector<Param> selectParams = queryObject.getSelectStatements();

		/* For storing clause results for pattern and such that clauses */
		vector<Clause> clauses;
		vector<Pattern> patterns;

		IntermediateTable iTable;

		/* Evaluation of clauses */
		for (Clause clause : queryObject.getClauses()) {
			ClauseResults clauseResults;
			evaluateClause(clause, clauseResults);

			if (!clauseResults.hasResults()) return{};
			if (clauseResults.numParamsInResult() != 0) EvaluatorHelper::mergeClauseTable(clauseResults, iTable);
		}

		/* Evaluation of patterns */
		for (Pattern clause : queryObject.getPatterns()) {
			ClauseResults patternResults;
			evaluatePattern(clause, patternResults);

			if (!patternResults.hasResults()) return{};
			if (patternResults.numParamsInResult() != 0) EvaluatorHelper::mergeClauseTable(patternResults, iTable);
		}
		return extractParams(selectParams, iTable);

	}
	else { // Return no value
		return{};
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
	else if (relation == Uses) {
		evaluateUses(clause, clauseResults);
	}
	else if (relation == Modifies) {
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

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateFollows(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();


	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollows();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBefore(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfter(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool results = pkb.checkFollows(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(results);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateFollowStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollowsStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfterStar(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkFollowsStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateParent(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) { // (syn, syn)
		if (Utils::isSynonym(rightParam.type)) {
			vector<vector<int>> results = pkb.getAllParent();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParent(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildren(stoi(leftParam.value));
			clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkParent(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateParentStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllParentStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParentStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildrenStar(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkParentStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

void QueryEvaluator::evaluateUses(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllStatementUsesVariables();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			if (rightParam.type == INTEGER) { // RHS is integer constant
				vector<vector<int>> results = pkb.getStatementsWithConstant(stoi(rightParam.value));
				clauseResults.setResults(results);
			}
			else { // LHS is var_name
				int variableId = pkb.getVariableId(rightParam.value);
				vector<vector<int>> results = pkb.getStatementsFromUsesVariable(variableId);
				clauseResults.setResults(results);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getUsesVariablesFromStatement(stoi(leftParam.value));
			clauseResults.setResults(results);
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

/* Right param: IDENT or stmt syn or proc syn */
/* Left param: var syn or IDENT or _ */
void QueryEvaluator::evaluateModifies(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllStatementModifiesVariables();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			int variableId = pkb.getVariableId(rightParam.value);
			vector<vector<int>> results = pkb.getStatementsFromModifiesVariable(variableId);
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getModifiesVariablesFromStatement(stoi(leftParam.value));
			clauseResults.setResults(results);
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
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateNext(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllNext(); //check PKB API
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			int variableId = pkb.getVariableId(rightParam.value);
			vector<vector<int>> results = pkb.getNextBefore(variableId); //check PKB API
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getNextAfter(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
		  //bool result;
			if (rightParam.type == IDENT) {
				int varId = pkb.getVariableId(rightParam.value);
				// result = pkb.checkNext(stoi(leftParam.value), varId);
			}
			else {
				// result = pkb.checkNext(stoi(leftParam.value), stoi(rightParam.value));
			}
			// clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateNextStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllNextStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getNextBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getNextAfterStar(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkNextStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateCalls(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) { // (syn, syn)
		if (Utils::isSynonym(rightParam.type)) {
			vector<vector<int>> results = pkb.getAllCalls();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBefore(stoi(rightParam.value));
			// clauseResults.setValues(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfter(stoi(leftParam.value));
			clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			// bool result = pkb.checkCall(stoi(leftParam.value), stoi(rightParam.value));
			// clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateCallsStar(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getFirstParam();
	Param rightParam = clause.getSecondParam();

	if (Utils::isSynonym(leftParam.type)) {
		if (Utils::isSynonym(rightParam.type)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllCallsStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfterStar(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkCallsStar(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* right Param: _ or IDENT or SYN */
/* left Param: _ or exprSpec */
void QueryEvaluator::evaluatePattern(Pattern & pattern, ClauseResults & patternResults)
{
	patternResults.instantiatePattern(pattern);

	vector<vector<int>> results = pkb.getPattern(pattern);
	patternResults.setResults(results);
};

/* Filters table for with assignment */
void QueryEvaluator::handleWithValueAssignment(Clause &clause, IntermediateTable &iTable) {
	Param lhs = clause.getFirstParam();
	Param rhs = clause.getSecondParam();
	int paramTableIndex = EvaluatorHelper::getParamInt(lhs, iTable);
	int paramValue;
	if (lhs.attribute == VALUE || lhs.attribute == STMT_NO) {
	//if (lhs.attribute == CONSTANT || lhs.attribute == VALUE || lhs.attribute == STMT_NO) {
		paramValue = stoi(rhs.value);
	} else {
		// Get pkb mapping from proc/var_name to int
		// paramValue = ...
	}

	vector<vector<int>> updatedTable;
	for (vector<int> tableRow : iTable.resultsTable) {
		if (tableRow[paramTableIndex] == paramValue) updatedTable.push_back(tableRow);
	}

	iTable.setResultsTable(updatedTable);
};

/* Filters table for with equating two variables */
void QueryEvaluator::handleWithEquateVariables(Clause &clause, IntermediateTable &iTable) {
	Param lhs = clause.getFirstParam();
	Param rhs = clause.getSecondParam();
	int firstParamTableIndex = EvaluatorHelper::getParamInt(lhs, iTable);
	int secondParamTableIndex = EvaluatorHelper::getParamInt(rhs, iTable);

	vector<vector<int>> updatedTable;
	for (vector<int> tableRow : iTable.resultsTable) {
		if (tableRow[firstParamTableIndex] == tableRow[secondParamTableIndex]) {
			updatedTable.push_back(tableRow);
		}
	}

	iTable.setResultsTable(updatedTable);
};

/* Returns the selected params from the intermediate table */
list<string> QueryEvaluator::extractParams(vector<Param> selectedParams, IntermediateTable &iTable) {
	if (selectedParams.size() == 1) {
		Param selected = selectedParams[0];
		if (selected.type == BOOLEAN) { // Boolean
			if (iTable.resultsTable.size() > 0) return{ "true" };
			else return{ "false" };
		}
		else { // Synonym
			return paramToStringList(selected, iTable);
		}
	}
	else { // Tuple
		list<string> tupleResult;
		for (Param p : selectedParams) {
			list<string> paramValues = paramToStringList(p, iTable);
			/* Concat string values as  */
			stringstream paramValSingleString;
			for (string val : paramValues) {
				paramValSingleString << val + " ";
			}
			paramValSingleString << ", ";
			tupleResult.push_back(paramValSingleString.str());
		}
		return tupleResult;
	}
};

list<string> QueryEvaluator::paramToStringList(Param p, IntermediateTable &iTable) {
	int paramInt = EvaluatorHelper::getParamInt(p, iTable);
	list<string> paramValues;
	for (vector<int> tableRow : iTable.resultsTable) {
		string paramVal;
		if (p.type == VARIABLE) {
			paramVal = pkb.getVariableName(tableRow[paramInt]);
		}
		else {
			paramVal = to_string(tableRow[paramInt]);
		}
		paramValues.push_back(paramVal);
	}
	return paramValues;
};
