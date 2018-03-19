#include "QueryEvaluator.h"

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

void QueryEvaluator::setInvalidQuery(string message) {
	validQuery = false;
	if (message != "") invalidQueryMessage = { message };
	else invalidQueryMessage = {};
};

/*
  Possible Optimizations: Check if clauseresults or table is empty at any point of time, if yes, return null
*/
list<string> QueryEvaluator::evaluateQuery() {
	if (isValidQuery()) {

		vector<Param> selectParams = queryObject.getSelectStatements();
		IntermediateTable iTable;

		/* Evaluation of clauses */
		for (Clause clause : queryObject.getClauses()) {
			ClauseResults clauseResults;
			evaluateClause(clause, clauseResults);
			if (!clauseResults.hasResults()) return{};
			clauseResults.removeALLSyns(); // Sanitization
			filterStmts(clauseResults);

			if (clauseResults.numParamsInResult() != 0) {
				EvaluatorHelper::mergeClauseTable(clauseResults, iTable);
			}
		}

		/* Evaluation of patterns */
		for (Pattern clause : queryObject.getPatterns()) {
			ClauseResults patternResults;
			evaluatePattern(clause, patternResults);
			if (!patternResults.hasResults()) return{};
			patternResults.removeALLSyns(); // Sanitization
			filterStmts(patternResults);

			if (patternResults.numParamsInResult() != 0) {
				EvaluatorHelper::mergeClauseTable(patternResults, iTable);
			}
		}

		/* Evaluation of with clauses */
		for (Clause withClause : queryObject.getWithClauses()) {
			handleWithClause(withClause, iTable);
		}

		return extractParams(selectParams, iTable);

	}
	else { // Return no value
		return invalidQueryMessage;
	}
}

/* Main evaluation method */
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
	else if (relation == Calls) {
		evaluateCalls(clause, clauseResults);
	}
	else if (relation == CallsT) {
		evaluateCallsStar(clause, clauseResults);
	}
	else { ; } // Extension
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
			vector<vector<int>> results;
			if (leftParam.type == PROCEDURE) { results = pkb.getAllProcedureUsesVariables();
			} else { results = pkb.getAllStatementUsesVariables(); }
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
			vector<vector<int>> results;
			int lineId;
			try { 
				lineId = stoi(leftParam.value);
				results = pkb.getUsesVariablesFromStatement(lineId);
			}
			catch (exception&) {
				lineId = pkb.getProcedureId(leftParam.value);
				results = pkb.getUsesVariablesFromProcedure(lineId);
			}
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result;

			int lineId;
			try { lineId = stoi(leftParam.value); }
			catch (exception&) { lineId = pkb.getProcedureId(leftParam.value); }

			int varId;
			try { varId = stoi(rightParam.value); }
			catch (exception&) { varId = pkb.getProcedureId(rightParam.value); }

			result = pkb.checkStatementUsesVariable(lineId, varId);
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
			vector<vector<int>> results;
			if (leftParam.type == PROCEDURE) { results = pkb.getAllProcedureModifiesVariables();
			} else { results = pkb.getAllStatementModifiesVariables(); }
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
			vector<vector<int>> results;
			int lineId;
			try { 
				lineId = stoi(leftParam.value);
				lineId = pkb.getProcedureId(leftParam.value);
			}
			catch (exception&) {
				results = pkb.getModifiesVariablesFromProcedure(lineId);
				results = pkb.getModifiesVariablesFromStatement(lineId);
			}
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result;

			int lineId;
			try { lineId = pkb.getProcedureId(leftParam.value); }
			catch (exception&) { lineId = stoi(leftParam.value); }

			int varId;
			try { varId = pkb.getVariableId(rightParam.value); }
			catch (exception&) { varId = stoi(rightParam.value); }

			result = pkb.checkStatementModifiesVariable(lineId, varId);
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
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam.type)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfter(stoi(leftParam.value));
			clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkCalls(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
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

	vector<vector<int>> results = pkb.getStatementsWithPattern(pattern);
	patternResults.setResults(results);
};

/* Filter stmts according to a/while/if */
void QueryEvaluator::filterStmts(ClauseResults &clauseResults) {
	if (clauseResults.numParamsInResult() == 2) {
		Param leftParam = clauseResults.tableParams[0];
		Param rightParam = clauseResults.tableParams[1];
		set<int> leftParamSet = getParamSet(leftParam);
		set<int> rightParamSet = getParamSet(rightParam);

		vector<vector<int>> newTable;
		for (vector<int> tableRow : clauseResults.results) {
			if (leftParam.value == rightParam.value) { // Same param value
				if (tableRow[0] == tableRow[1] &&
					leftParamSet.find(tableRow[0]) != leftParamSet.end()) {
					newTable.push_back(tableRow);
				}
			} else if (leftParamSet.find(tableRow[0]) != leftParamSet.end() &&
				rightParamSet.find(tableRow[1]) != rightParamSet.end()) {
				newTable.push_back(tableRow);
			} else { ; }
		}

		clauseResults.setResults(newTable);

	}
	else if (clauseResults.numParamsInResult() == 1) {
		set<int> paramSet = getParamSet(clauseResults.tableParams[0]);

		vector<vector<int>> newTable;
		for (vector<int> tableRow : clauseResults.results) {
			if (paramSet.find(tableRow[0]) != paramSet.end()) {
				newTable.push_back(tableRow);
			}
		}

		clauseResults.setResults(newTable);
	}
	else { ; }
};

/* Get param type as a set of values */
set<int> QueryEvaluator::getParamSet(Param p) {
	ParamType pType = p.type;
	vector<vector<int>> results;
	set<int> paramSet;

	if (pType == VARIABLE) { results = pkb.getAllVariables(); }
	else if (pType == STMT || pType == PROG_LINE) { results = pkb.getAllStatements(); }
	else if (pType == ASSIGN) { results = pkb.getAllStatementsWithType(1); }
	else if (pType == WHILE) { results = pkb.getAllStatementsWithType(2); }
	else if (pType == IF) { results = pkb.getAllStatementsWithType(3); }
	else if (pType == PROCEDURE) { results = pkb.getAllProcedures(); }
	else if (pType == CONSTANT) { results = pkb.getAllConstants(); }
	else { ; }

	for (vector<int> values : results) {
		paramSet.insert(values[0]);
	}

	return paramSet;
}

/* Filters table for with assignment */
void QueryEvaluator::handleWithClause(Clause &clause, IntermediateTable &iTable) {

	if (EvaluatorHelper::withClauseNumSyns(clause, iTable) == 1) {
		handleWithValueAssignment(clause, iTable);
	}
	else if (EvaluatorHelper::withClauseNumSyns(clause, iTable) == 2) {
		handleWithEquateVariables(clause, iTable);
	}
	else { // Evaluate with by itself
		iTable.hasResults = handleWithEvaluation(clause, iTable);
	}
};

/* Filters table rows according to assigned value of param */
void QueryEvaluator::handleWithValueAssignment(Clause &clause, IntermediateTable &iTable) {

	Param paramInTable;
	Param paramWithValue;
	if (Utils::isSynonym(clause.getFirstParam().type)) {
		paramInTable = clause.getFirstParam();
		paramWithValue = clause.getSecondParam();
	}
	if (Utils::isSynonym(clause.getSecondParam().type)) {
		paramInTable = clause.getSecondParam();
		paramWithValue = clause.getFirstParam();
	}

	int paramIndex;
	paramIndex = EvaluatorHelper::getParamInt(paramInTable, iTable);

	int valueOfParam;
	if (paramInTable.attribute == PROCNAME) { // Constant is procedure name
		valueOfParam = pkb.getVariableId(paramWithValue.value);
	}
	else if (paramInTable.attribute == VARNAME) { // Constant is variable name
		valueOfParam = pkb.getProcedureId(paramWithValue.value);
	}
	else { // Constant is integer
		valueOfParam = stoi(paramWithValue.value);
	}

	vector<vector<int>> updatedTable;
	for (vector<int> tableRow : iTable.resultsTable) {
		if (tableRow[paramIndex] == valueOfParam) updatedTable.push_back(tableRow);
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
		int lhsIntValue = tableRow[firstParamTableIndex];
		int rhsIntValue = tableRow[secondParamTableIndex];

		if (lhs.attribute == PROCNAME || lhs.attribute == VARNAME) { // Compare string value
			if (getProcOrVarName(lhs.attribute, lhsIntValue) ==
				getProcOrVarName(rhs.attribute, rhsIntValue)) {
				updatedTable.push_back(tableRow);
			}
		}
		else if (tableRow[firstParamTableIndex] == tableRow[secondParamTableIndex]) { // Compare int value
			updatedTable.push_back(tableRow);
		}
		else { ; }

	}
	iTable.setResultsTable(updatedTable);
};

/* Handles with evaluation when not found in table */
bool QueryEvaluator::handleWithEvaluation(Clause &withClause, IntermediateTable &iTable) {
	Param lhs = withClause.getFirstParam();
	Param rhs = withClause.getSecondParam();
	vector<vector<int>> results; // For storage into iTable
	if (Utils::isSynonym(lhs.type) && Utils::isSynonym(rhs.type)) { // Both are synonyms
		set<int> lhsParamSet = getParamSet(lhs);
		set<int> rhsParamSet = getParamSet(rhs);
		for (int value : lhsParamSet) {
			if (rhsParamSet.find(value) != rhsParamSet.end()) return true;
		}
	} else if (Utils::isSynonym(lhs.type)) { // LHS syn
		set<int> lhsParamSet = getParamSet(lhs);
		for (int value : lhsParamSet) {
			if (value == getId(rhs)) return true;
		}
	} else if (Utils::isSynonym(rhs.type)) { // RHS syn
		set<int> rhsParamSet = getParamSet(rhs);
		for (int value : rhsParamSet) {
			if (value == getId(lhs)) return true;
		}
	}
	else {
		return lhs.value == rhs.value;
	}
	return false;
}

/* Returns id for string value */
int QueryEvaluator::getId(Param p) {
	if (p.type == PROCEDURE) {
		return pkb.getProcedureId(p.value);
	} else if (p.type == VARNAME) {
		return pkb.getVariableId(p.value);
	} else { // Assumes integer value for other types
		return stoi(p.value);
	}
};

/* Returns the name of the procedure or variable id */
string QueryEvaluator::getProcOrVarName(AttrType type, int id) {
	if (type == PROCNAME) {
		return pkb.getProcedureName(id);
	}
	else if (type == VARNAME) {
		return pkb.getVariableName(id);
	}
	else {
		return "";
	}
};

/* Returns the selected params from the intermediate table */
list<string> QueryEvaluator::extractParams(vector<Param> selectedParams, IntermediateTable &iTable) {
	if (!iTable.hasResults) {
		return{};
	} else if (selectedParams.size() == 1) {
		Param selected = selectedParams[0];
		if (selected.type == BOOLEAN) { // Boolean
			if (iTable.resultsTable.size() > 0 || // Table not empty
				iTable.tableParams.size() == 0) { // No statement to evaluate
				return{ "true" };
			}
			else {
				return{ "false" };
			}
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
	set<string> paramValueSet;
	list<string> paramValues;

	if (paramInt > -1) { // Selected Param is in table
		/* Store values into set */
		for (vector<int> tableRow : iTable.resultsTable) {
			string paramVal;
			if (p.type == VARIABLE) {
				paramVal = pkb.getVariableName(tableRow[paramInt]);
			}
			else if (p.type == PROCEDURE) {
				paramVal = pkb.getProcedureName(tableRow[paramInt]);
			}
			else {
				paramVal = to_string(tableRow[paramInt]);
			}
			paramValueSet.insert(paramVal);
		}

		/* Transfer set values into list */
		for (string value : paramValueSet) {
			paramValues.push_back(value);
		}
		return paramValues;
	}
	else { // Selected param not in table
		return getAllParamsOfType(p);
	}
};

/* Returns the values of given param */
list<string> QueryEvaluator::getAllParamsOfType(Param p) {
	set<int> paramSet = getParamSet(p);
	ParamType pType = p.type;
	list<string> allParams;

	for (int value : paramSet) {
		string valueString;
		if (pType == VARIABLE) { valueString = pkb.getVariableName(value); }
		else if (pType == PROCEDURE) { valueString = pkb.getProcedureName(value); }
		else { valueString = to_string(value); }
		allParams.push_back(valueString);
	}

	return allParams;
};
