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

/*
  Possible Optimizations: Check if clauseresults or table is empty at any point of time, if yes, return null
*/
list<string> QueryEvaluator::evaluateQuery() {
	if (isValidQuery()) {

		/* Push clauses into one vector for sorting */
		vector<Clause> consolidatedClauses;
		QueryOptimization::consolidateClauses(queryObject.getClauses(), consolidatedClauses);
		QueryOptimization::consolidateClauses(queryObject.getPatterns(), consolidatedClauses);
		QueryOptimization::consolidateClauses(queryObject.getWithClauses(), consolidatedClauses);

		// Grouping according to syns
		map<int, vector<Clause>> sortedClauses = QueryOptimization::sortIntoGroups(consolidatedClauses);

		/* Convert to vector<Clause> */
		vector<vector<Clause>> sortedClausesVector;
		for (pair<int, vector<Clause>> groupedClauses : sortedClauses) {
			sortedClausesVector.push_back(groupedClauses.second);
		}

		QueryOptimization qo;

		map<int, vector<Clause>> groupsAfterSortingByConstants;
		for (vector<Clause> reGroupedClauses : sortedClausesVector) {
			map<int, vector<Clause>> groupsAfterSortingByConstants = qo.numConstantsGroupClauses(reGroupedClauses);
		}

		vector<vector<Clause>> sortNumResults;
		for (pair<int, vector<Clause>> constantGroupedClauses : groupsAfterSortingByConstants) {
			sortNumResults.push_back(constantGroupedClauses.second);
		}

		map<int, vector<Clause>> groupsAfterSortingByNumResults;
		for (vector<Clause> groupByConstants : sortNumResults) {
			vector<Clause> groupsAfterSortingByNumResults = qo.numResultsGroupClauses(groupByConstants);
		}

		vector<Param> selectParams = queryObject.getSelectStatements(); // Selected Params
		map<Clause, vector<vector<int>>> cache; // For cached results

		vector<IntermediateTable> tables;
		for (pair<int, vector<Clause>> groupedClauses : sortedClauses) {

			IntermediateTable iTable; // Instantiate table for each group
			iTable.instantiateTable();
			// Evaluate each clause within group
			for (Clause clause : groupedClauses.second) {
				ClauseResults clauseResults;
				evaluateClauseGeneral(clause, clauseResults, iTable, cache);
			}
			if (!iTable.tableHasResults()) {
				if (selectParams[0].type == BOOLEAN) { // Short circuit if no results
					return{ "false" };
				} else {
					return{};
				}
			} else {
				tables.push_back(iTable);
			}
		}
		cache.clear();

		// To be refactored...
		return extractParams(selectParams, tables);

	}
	else { // Return no value
		return invalidQueryMessage;
	}
}

// General evaluation method for base Clause type
void QueryEvaluator::evaluateClauseGeneral(Clause &clause, ClauseResults &clauseResults,
	IntermediateTable &iTable, map<Clause, vector<vector<int>>> &cache) {
	if (clause.getRelRef() == With) { // With
		handleWithClause(clause, iTable);
	} else {
		if (clause.getRelRef() == None) { // Pattern
			Pattern* pattern = static_cast<Pattern*>(&clause);
			evaluatePattern(*pattern, clauseResults);
		}
		else { // Such That
			clauseResults.instantiateClause(clause);
			if (EvaluatorHelper::clauseInCache(clause, cache)) { // Exact clause cached
				clauseResults.setResults(cache[clause]);;
			} else {
				if (EvaluatorHelper::unsanitizedClauseInCache(clause, cache)) { // General clause cached
					vector<vector<int>> cachedResults = cache[EvaluatorHelper::generalizeClause(clause)];
					clauseResults.setResults(cachedResults);;
				} else { // Uncached at all
					evaluateClause(clause, clauseResults);
					EvaluatorHelper::cacheUnsanitized(clause, clauseResults, cache);
				}
				clauseResults.removeALLSyns(); // Sanitization
				filterStmts(clauseResults); // Filter while/if/assign
				EvaluatorHelper::cacheSanitized(clause, clauseResults, cache);
			}
		}
		// EvaluatorHelper::storeSanitized();
		EvaluatorHelper::mergeClauseTable(clauseResults, iTable);
	}
};

/* Main evaluation method */
void QueryEvaluator::evaluateClause(Clause & clause, ClauseResults & clauseResults)
{
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
	else if (relation == Affects) {
		evaluateAffects(clause, clauseResults);
	}
	else if (relation == AffectsT) {
		evaluateAffectsStar(clause, clauseResults);
	}
	else { ; } // for bonus feature?
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateFollows(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollows();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBefore(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollowsStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) { // (syn, syn)
		if (Utils::isSynonym(rightParam)) {
			vector<vector<int>> results = pkb.getAllParent();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParent(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllParentStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParentStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
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
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results;
			if (leftParam.type == PROCEDURE) {
				results = pkb.getAllProcedureModifiesVariables();
			} else {
				results = pkb.getAllStatementModifiesVariables();
			}
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			int variableId = pkb.getVariableId(rightParam.value);
			vector<vector<int>> results = pkb.getStatementsFromModifiesVariable(variableId);
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllNext(); //check PKB API
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getNextBefore(stoi(rightParam.value)); //check PKB API
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getNextAfter(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
		  bool result;
			if (rightParam.type == IDENT) {
				int varId = pkb.getVariableId(rightParam.value);
				result = pkb.checkNext(stoi(leftParam.value), varId);
			}
			else {
				result = pkb.checkNext(stoi(leftParam.value), stoi(rightParam.value));
			}
			clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateNextStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllNextStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getNextBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
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
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) { // (syn, syn)
		if (Utils::isSynonym(rightParam)) {
			vector<vector<int>> results = pkb.getAllCalls();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBefore(pkb.getProcedureId(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfter(pkb.getProcedureId(leftParam.value));
			clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkCalls(pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateCallsStar(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllCallsStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBeforeStar(pkb.getProcedureId(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfterStar(pkb.getProcedureId(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkCallsStar(pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: assignment syn or assignment no or _ */
/* Left param: assignment syn or assignment no or _ */
void QueryEvaluator::evaluateAffects(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) { // (syn, syn)
		if (Utils::isSynonym(rightParam)) {
			vector<vector<int>> results = pkb.getAllAffects();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getAffectsBefore(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getAffectsAfter(stoi(leftParam.value));
			clauseResults.setResults(results);

		}
		else { // (concrete, conrete)
			bool result = pkb.checkAffects(stoi(leftParam.value), stoi(rightParam.value));
			clauseResults.setValid(result);
		}
	}
}

/* Right param: assignment syn or assignment no or _ */
/* Left param: assignment syn or assignment no or _ */
void QueryEvaluator::evaluateAffectsStar(Clause & clause, ClauseResults & clauseResults)
{
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllAffectsStar();
			clauseResults.setResults(results);
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getAffectsBeforeStar(stoi(rightParam.value));
			clauseResults.setResults(results);
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getAffectsAfterStar(stoi(leftParam.value));
			clauseResults.setResults(results);
		}
		else { // (concrete, concrete)
			bool result = pkb.checkAffectsStar(stoi(leftParam.value), stoi(rightParam.value));
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
	AttrType pAttr = p.attribute;
	vector<vector<int>> results;
	set<int> paramSet;

	if (pType == VARIABLE) { results = pkb.getAllVariables(); }
	else if (pType == STMT || pType == PROG_LINE) { results = pkb.getAllStatements(); }
	else if (pType == ASSIGN) { results = pkb.getAllStatementsWithType(1); }
	else if (pType == WHILE) { results = pkb.getAllStatementsWithType(2); }
	else if (pType == IF) { results = pkb.getAllStatementsWithType(3); }
	else if (pType == CALL) {
		vector<vector<int>> callStmts = pkb.getAllStatementsWithType(4);
		if (pAttr == NONE) results = callStmts;
		else if (pAttr == PROCNAME) {
			for (vector<int> stmt : callStmts) {
				vector<int> procId = pkb.getProcedureCalledByCallStatement(stmt[0])[0];
				results.push_back(procId);
			}
		}
	}
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
	else { // Evaluate and store into with table
		iTable.hasResults = handleWithEvaluation(clause, iTable);
	}
};

/* Filters table rows according to assigned value of param */
void QueryEvaluator::handleWithValueAssignment(Clause &clause, IntermediateTable &iTable) {

	Param paramInTable;
	Param paramWithValue;
	if (Utils::isSynonym(clause.getLeftParam())) {
		paramInTable = clause.getLeftParam();
		paramWithValue = clause.getRightParam();
	}
	if (Utils::isSynonym(clause.getRightParam())) {
		paramInTable = clause.getRightParam();
		paramWithValue = clause.getLeftParam();
	}

	int paramIndex;
	paramIndex = iTable.getParamIndex(paramInTable);

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
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();
	int firstParamTableIndex = iTable.getParamIndex(lhs);
	int secondParamTableIndex = iTable.getParamIndex(rhs);

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
	Param lhs = withClause.getLeftParam();
	Param rhs = withClause.getRightParam();
	ClauseResults withClauseResults;
	vector<vector<int>> withResults;

	withClauseResults.instantiateClause(withClause);
	if (Utils::isSynonym(lhs) && Utils::isSynonym(rhs)) { // Both are synonyms
		set<int> lhsParamSet = getParamSet(lhs);
		set<int> rhsParamSet = getParamSet(rhs);

		for (int value : lhsParamSet) {
			vector<int> withTableRow;
			if (rhsParamSet.find(value) != rhsParamSet.end()) {
				withTableRow.push_back(value); withTableRow.push_back(value);
				withResults.push_back(withTableRow);
			}
		}
		if (withResults.size() > 0) {
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
			return true;
		}

	} else if (Utils::isSynonym(lhs)) { // LHS syn
		set<int> lhsParamSet = getParamSet(lhs);

		for (int value : lhsParamSet) {
			vector<int> withTableRow;
			withTableRow.clear();
			if (value == getId(rhs, lhs.type, lhs.attribute)) {
				withTableRow.push_back(value);
				withResults.push_back(withTableRow);
			}
		}

		if (withResults.size() > 0) {
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
			return true;
		}

	} else if (Utils::isSynonym(rhs)) { // RHS syn
		set<int> rhsParamSet = getParamSet(rhs);

		for (int value : rhsParamSet) {
			vector<int> withTableRow;
			if (value == getId(lhs, rhs.type, rhs.attribute)) {
				withTableRow.push_back(value);
			}
		}

		if (withResults.size() > 0) {
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
			return true;
		}
	}
	else {
		return lhs.value == rhs.value;
	}
	return false;
}

/* Returns id for string value */
int QueryEvaluator::getId(Param p, ParamType type, AttrType attribute) {
	if (type == PROCEDURE) {
		return pkb.getProcedureId(p.value);
	}
	else if (type == CALL) {
		if (attribute == PROCNAME) {
			return pkb.getProcedureId(p.value);
		}
		else {
			return stoi(p.value);
		}
	}
	else if (type == VARIABLE) {
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
list<string> QueryEvaluator::extractParams(vector<Param> selectedParams, vector<IntermediateTable> &iTables) {
	if (selectedParams.size() == 1) {
		Param selected = selectedParams[0];
		IntermediateTable tableWithParam = EvaluatorHelper::findTableWithParam(selected, iTables);
		if (selected.type == BOOLEAN) { // Select Boolean
			if (tableWithParam.tableHasResults()) {
				return{ "true" };
			} else {
				return{ "false" };
			}
		}
		else { // Select Synonym
			return paramToStringList(selected, tableWithParam);
		}
	} else { // Select Tuple
		set<string> tupleResultSet;
		list<string> tupleResult;

		IntermediateTable mergedTable;
		mergedTable.instantiateTable();

		// Merge tables for tuples
		for (Param p : selectedParams) {
			IntermediateTable tableWithParam = EvaluatorHelper::findTableWithParam(p, iTables);
			mergedTable = EvaluatorHelper::mergeIntermediateTables(tableWithParam, mergedTable);
		}

		stringstream tupleRowString;
		// Iterate through all rows
		for (size_t i = 0; i < mergedTable.resultsTable.size(); i++) {
			vector<int> tableRow = mergedTable.resultsTable[i];
			tupleRowString.str("");

			// Iterate through each value of row
			for (size_t j = 0; j < selectedParams.size(); j++) {
				int indexOfParam = mergedTable.getParamIndex(selectedParams[j]);
				int paramValue = tableRow[indexOfParam];

				string value;
				if (selectedParams[j].type == PROCEDURE) {
					value = pkb.getProcedureName(paramValue);
				} else if (selectedParams[j].type == VARIABLE) {
					value = pkb.getVariableName(paramValue);
				} else {
					value = to_string(tableRow[indexOfParam]);
				}

				if (j == selectedParams.size() - 1) tupleRowString << value;
				else tupleRowString << value << " ";
			}
			tupleResultSet.insert(tupleRowString.str());
		}

		for (string ans : tupleResultSet) {
			tupleResult.push_back(ans);
		}
		return tupleResult;
	}
};

list<string> QueryEvaluator::paramToStringList(Param p, IntermediateTable &iTable) {
	int paramInt = iTable.getParamIndex(p);
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
	else if (iTable.tableHasResults()) { // Selected param not in table
		return getAllParamsOfType(p);
	} else {
		return{}; // Results table is empty
	}
};

/* Returns the values of given param */
list<string> QueryEvaluator::getAllParamsOfType(Param p) {
	set<int> paramSet = getParamSet(p);
	ParamType pType = p.type;
	AttrType pAttr = p.attribute;
	list<string> allParams;

	for (int value : paramSet) {
		string valueString;
		if (pType == VARIABLE) { valueString = pkb.getVariableName(value); }
		else if (pType == PROCEDURE) { valueString = pkb.getProcedureName(value); }
		else if (pType == CALL && pAttr == PROCNAME) { valueString = pkb.getProcedureName(value);  }
		else { valueString = to_string(value); }
		allParams.push_back(valueString);
	}

	return allParams;
};
