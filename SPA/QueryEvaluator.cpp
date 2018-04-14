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

/* Main query evaluation method */
list<string> QueryEvaluator::evaluateQuery() {
	if (isValidQuery()) {

		/* Push clauses into one vector for sorting */
		vector<Clause> consolidatedClauses;
		QueryOptimization::consolidateClauses(queryObject.getClauses(), consolidatedClauses);
		QueryOptimization::consolidateClauses(queryObject.getPatterns(), consolidatedClauses);
		QueryOptimization::consolidateClauses(queryObject.getWithClauses(), consolidatedClauses);

		// Grouping according to syns
		map<int, vector<Clause>> sortedClauses = QueryOptimization::sortIntoGroups(consolidatedClauses);
		// Sort intra-group
		vector<vector<Clause>> sortedIntraGroupsClauses;
		for (pair<int, vector<Clause>> sortedClause : sortedClauses) {
			vector<Clause> groupSortedClass = QueryOptimization::sortWithinGroup(sortedClause.second, pkb);
			sortedIntraGroupsClauses.push_back(groupSortedClass);
		}
		// Sort for evaluation order of groups by total weight
		vector<vector<Clause>> sortedGroupEvalOrder = QueryOptimization::sortGroupsEvalOrder(sortedIntraGroupsClauses, pkb);

		vector<Param> selectParams = queryObject.getSelectStatements(); // Selected Params
		map<Clause, vector<vector<int>>> cache; // For cached results

		/* ACTUAL EVALUATION */
		vector<IntermediateTable> tables;
		for (vector<Clause> groupedClauses : sortedGroupEvalOrder) {

			IntermediateTable iTable; // Instantiate table for each group
			iTable.instantiateTable();

			// Evaluate each clause within group and merge
			int numClauses = groupedClauses.size();
			vector<Clause>::iterator clauseDeletedIt;
			for (size_t i = 0; i < numClauses; i++) {
				ClauseResults clauseResults;
				if (groupedClauses.size() == numClauses) { // Evalute first clause
					evaluateClauseGeneral(groupedClauses[0], clauseResults, iTable, cache);
					clauseDeletedIt = groupedClauses.begin();
					groupedClauses.erase(clauseDeletedIt);
				} else {
					int indexOfClauseEvaluated = 0;
					for (size_t j = 0; j < groupedClauses.size(); j++) {
						if (EvaluatorHelper::clauseParamsInTable(groupedClauses[j], iTable)) { // Next clause has overlapping syn
							evaluateClauseGeneral(groupedClauses[j], clauseResults, iTable, cache);
							indexOfClauseEvaluated = j;
							break;
						} else if (j == groupedClauses.size() - 1) { // Last clause to evaluate
							evaluateClauseGeneral(groupedClauses[j], clauseResults, iTable, cache);
							indexOfClauseEvaluated = j;
							break;
						} else { // No overlapping syn and more clauses at the back
							if (groupedClauses[j + 1].getRelRef() == Affects ||
								groupedClauses[j + 1].getRelRef() == AffectsT) { // Evaluate current if next is affects or affectsStar
								evaluateClauseGeneral(groupedClauses[j], clauseResults, iTable, cache);
								indexOfClauseEvaluated = j;
								break;
							}
						}
					}
					clauseDeletedIt = groupedClauses.begin() + indexOfClauseEvaluated;
					groupedClauses.erase(clauseDeletedIt);
				}
				if (!iTable.tableHasResults()) {
					break;
				}
			}

			//for (Clause clause : groupedClauses) {
			//	ClauseResults clauseResults;
			//	evaluateClauseGeneral(clause, clauseResults, iTable, cache);
			//}

			// Short circuit for no results
			if (!iTable.tableHasResults()) {
				if (selectParams[0].type == BOOLEAN) return{ "false" };
				else return{};
			} else { // Push back results
				tables.push_back(iTable);
			}
		}
		cache.clear();

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

	// If same param (not ALL)
	if (Utils::isSameParam(leftParam, rightParam) && leftParam.type != ALL && rightParam.type != ALL) {
		if (clause.getIsInverted()) clauseResults.setValid(true); // Clauseresults defaults false
		return;
	}

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollows();
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else { // NOT case
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBefore(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else { // NOT case
				vector<vector<int>> universeResults = pkb.getAllFollows();
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else { //concrete, 
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfter(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else { // NOT case
				vector<vector<int>> universeResults = pkb.getAllFollows();
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool results = pkb.checkFollows(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!results) : clauseResults.setValid(results);
		}
	}
}


/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateFollowStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	// If same param (not ALL)
	if (Utils::isSameParam(leftParam, rightParam) && leftParam.type != ALL && rightParam.type != ALL) {
		if (clause.getIsInverted()) clauseResults.setValid(true); // Clauseresults defaults false
		return;
	}

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllFollowsStar();
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else { // NOT case
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getFollowsBeforeStar(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getFollowsAfterStar(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else { // NOT case
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool results = pkb.checkFollowsStar(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!results) : clauseResults.setValid(results);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateParent(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	// If same param (not ALL)
	if (Utils::isSameParam(leftParam, rightParam) && leftParam.type != ALL && rightParam.type != ALL) {
		if (clause.getIsInverted()) clauseResults.setValid(true); // Clauseresults defaults false
		return;
	}

	if (Utils::isSynonym(leftParam)) { // (syn, syn)
		if (Utils::isSynonym(rightParam)) {
			vector<vector<int>> results = pkb.getAllParent();
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParent(stoi(rightParam.value));
			if (results[0][0] == 0) {
				results.clear();
			}
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildren(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, conrete)
			bool result = pkb.checkParent(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
		}
	}
}

/* Right param: stmt syn or stmt no or _ */
/* Left param: stmt syn or stmt no or _ */
void QueryEvaluator::evaluateParentStar(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	// If same param (not ALL)
	if (Utils::isSameParam(leftParam, rightParam) && leftParam.type != ALL && rightParam.type != ALL) {
		if (clause.getIsInverted()) clauseResults.setValid(true); // Clauseresults defaults false
		return;
	}

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results = pkb.getAllParentStar();
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getParentStar(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getChildrenStar(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool result = pkb.checkParentStar(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
		}
	}
}

void QueryEvaluator::evaluateUses(Clause & clause, ClauseResults & clauseResults) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();

	if (Utils::isSynonym(leftParam)) {
		if (Utils::isSynonym(rightParam)) { // (syn, syn)
			vector<vector<int>> results;
			results = (leftParam.type == PROCEDURE) ? pkb.getAllProcedureUsesVariables() : 
				pkb.getAllStatementUsesVariables();

			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results;
			if (rightParam.type == INTEGER) { // RHS is integer constant
				results = (leftParam.type == PROCEDURE) ?
					pkb.getProceduresWithConstant(stoi(rightParam.value)) :
					pkb.getStatementsWithConstant(stoi(rightParam.value));
			} else { // LHS is var_name
				int variableId = pkb.getVariableId(rightParam.value);
				results = (leftParam.type == PROCEDURE) ?
					pkb.getProceduresFromUsesVariable(variableId) : 
					pkb.getStatementsFromUsesVariable(variableId);
			}

			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results;
			int lineId;
			if (leftParam.type == PROC_IDENT) {
				lineId = pkb.getProcedureId(leftParam.value);
				results = pkb.getUsesVariablesFromProcedure(lineId);
			} else {
				lineId = stoi(leftParam.value);
				results = pkb.getUsesVariablesFromStatement(lineId);
			}
			
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
		else { // (concrete, concrete)
			bool result;
			int varId = pkb.getVariableId(rightParam.value);
			int lineId;

			if (leftParam.type == PROC_IDENT) {
				lineId = pkb.getProcedureId(leftParam.value);
				result = pkb.checkProcedureUsesVariable(lineId, varId);
			} else {
				lineId = stoi(leftParam.value);
				result = pkb.checkStatementUsesVariable(lineId, varId);
			}

			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			results = (leftParam.type == PROCEDURE) ? pkb.getAllProcedureModifiesVariables() :
				pkb.getAllStatementModifiesVariables();

			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		}
		else { // (syn, concrete)
			int variableId = pkb.getVariableId(rightParam.value);
			vector<vector<int>> results;
			results = (leftParam.type == PROCEDURE) ?
				pkb.getProceduresFromModifiesVariable(variableId):
				pkb.getStatementsFromModifiesVariable(variableId);

			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results;
			int lineId;
			if (leftParam.type == PROC_IDENT) {
				lineId = pkb.getProcedureId(leftParam.value);
				results = pkb.getModifiesVariablesFromProcedure(lineId);
			} else {
				lineId = stoi(leftParam.value);
				results = pkb.getModifiesVariablesFromStatement(lineId);
			}

			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
		else { // (concrete, concrete)
			bool result;
			int varId = pkb.getVariableId(rightParam.value);
			int lineId;

			if (leftParam.type == PROC_IDENT) {
				lineId = pkb.getProcedureId(leftParam.value);
				result = pkb.checkProcedureModifiesVariable(lineId, varId);
			} else {
				lineId = stoi(leftParam.value);
				result = pkb.checkStatementModifiesVariable(lineId, varId);
			}

			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}

		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getNextBefore(stoi(rightParam.value)); //check PKB API
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getNextAfter(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool result = pkb.checkNext(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getNextBeforeStar(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getNextAfterStar(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool result = pkb.checkNextStar(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBefore(pkb.getProcedureId(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfter(pkb.getProcedureId(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, conrete)
			bool result = pkb.checkCalls(pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getCallsBeforeStar(pkb.getProcedureId(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getCallsAfterStar(pkb.getProcedureId(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		} else { // (concrete, concrete)
			bool result = pkb.checkCallsStar(pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		} else { // (syn, concrete)
			vector<vector<int>> results = pkb.getAffectsBefore(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	} else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getAffectsAfter(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
		else { // (concrete, conrete)
			bool result = pkb.checkAffects(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
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
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = universeSet(leftParam, rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetDouble(clauseResults, results);
			}
		}
		else { // (syn, concrete)
			vector<vector<int>> results = pkb.getAffectsBeforeStar(stoi(rightParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(leftParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
	}
	else {
		if (Utils::isSynonym(rightParam)) { // (concrete, syn)
			vector<vector<int>> results = pkb.getAffectsAfterStar(stoi(leftParam.value));
			if (!clause.getIsInverted()) {
				clauseResults.setResults(results);
			} else {
				vector<vector<int>> universeResults = setValuesforParam(rightParam);
				clauseResults.setResults(universeResults);
				EvaluatorHelper::subtractSetSingle(clauseResults, results);
			}
		}
		else { // (concrete, concrete)
			bool result = pkb.checkAffectsStar(stoi(leftParam.value), stoi(rightParam.value));
			clause.getIsInverted() ? clauseResults.setValid(!result) : clauseResults.setValid(result);
		}
	}
} 

/* right Param: _ or IDENT or SYN */
/* left Param: _ or exprSpec */
void QueryEvaluator::evaluatePattern(Pattern & pattern, ClauseResults & patternResults) {
	patternResults.instantiatePattern(pattern);

	vector<vector<int>> results = pkb.getStatementsWithPattern(pattern);
	if (!pattern.getIsInverted()) {
		patternResults.setResults(results);
	} else {
		if (patternResults.tableParams.size() == 2) { // Double intersect (Entity, leftParam)
			vector<vector<int>> universeResults = universeSet(pattern.getEntity(), pattern.getLeftParam());
			patternResults.setResults(universeResults);
			EvaluatorHelper::subtractSetDouble(patternResults, results);
		} else { // Single intersect (Entity)
			vector<vector<int>> universeResults = setValuesforParam(pattern.getEntity());
			patternResults.setResults(universeResults);
			EvaluatorHelper::subtractSetSingle(patternResults, results);
		}
	}
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
		else if (pAttr == STMT_NO) results = callStmts;
		else if (pAttr == PROCNAME) {
			for (vector<int> stmt : callStmts) {
				vector<int> procId = pkb.getProcedureCalledByCallStatement(stmt[0])[0];
				results.push_back(procId);
			}
		}
	}
	else if (pType == PROCEDURE) { results = pkb.getAllProcedures(); }
	else if (pType == CONSTANT) { results = pkb.getAllConstants(); }
	else if (pType == STMTLST) { results = pkb.getAllStatementListsFirstStmt(); }
	else { ; }

	for (vector<int> values : results) {
		paramSet.insert(values[0]);
	}

	return paramSet;
}

/* Filters table for with assignment */
void QueryEvaluator::handleWithClause(Clause &clause, IntermediateTable &iTable) {
	if (Utils::compareParam(clause.getLeftParam(), clause.getRightParam())) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults;
		Param p = clause.getLeftParam();
		p.attribute = NONE;
		set<int> results = getParamSet(p);
		for (int i : results) {
			withResults.push_back({ i });
		}

		withClauseResults.setResults(withResults);
		withClauseResults.setValid(true);
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if ((clause.getLeftParam().attribute == STMT_NO
		&& clause.getRightParam().type == CONSTANT) ||
		(clause.getRightParam().attribute == STMT_NO
			&& clause.getLeftParam().type == CONSTANT)) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);

		if (clause.getRightParam().type == CONSTANT) {
			int type;
			if (clause.getLeftParam().type == STMT) type = 0;
			else if (clause.getLeftParam().type == ASSIGN) type = 1;
			else if (clause.getLeftParam().type == WHILE) type = 2;
			else if (clause.getLeftParam().type == IF) type = 3;
			else if (clause.getLeftParam().type == CALL) type = 4;
			vector<vector<int>> withResults = pkb.getWithStmtNoConstValue(type);
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		}
		else {
			int type;
			if (clause.getRightParam().type == STMT) type = 0;
			else if (clause.getRightParam().type == ASSIGN) type = 1;
			else if (clause.getRightParam().type == WHILE) type = 2;
			else if (clause.getRightParam().type == IF) type = 3;
			else if (clause.getRightParam().type == CALL) type = 4;
			vector<vector<int>> withResults = pkb.getWithStmtNoConstValue(type);
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		}
	}
	else if (clause.getLeftParam().type == PROCEDURE
		&& clause.getRightParam().type == VARIABLE) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithProcNameVarName();
		withClauseResults.setResults(withResults);
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getLeftParam().type == PROCEDURE
		&& clause.getRightParam().type == CALL
		&& clause.getRightParam().attribute == PROCNAME) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithProcNameCallProcName();
		withClauseResults.setResults(withResults);
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getLeftParam().type == VARIABLE
		&& clause.getRightParam().type == CALL
		&& clause.getRightParam().attribute == PROCNAME) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithVarNameCallProcName();
		withClauseResults.setResults(withResults);
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getRightParam().type == PROCEDURE
		&& clause.getLeftParam().type == VARIABLE) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithProcNameVarName();
		withClauseResults.setResults(Utils::invertTwoValues(withResults));
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getRightParam().type == PROCEDURE
		&& clause.getLeftParam().type == CALL
		&& clause.getLeftParam().attribute == PROCNAME) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithProcNameCallProcName();
		withClauseResults.setResults(Utils::invertTwoValues(withResults));
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getRightParam().type == VARIABLE
		&& clause.getLeftParam().type == CALL
		&& clause.getLeftParam().attribute == PROCNAME) {
		ClauseResults withClauseResults;
		withClauseResults.instantiateClause(clause);
		vector<vector<int>> withResults = pkb.getWithVarNameCallProcName();
		withClauseResults.setResults(Utils::invertTwoValues(withResults));
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
	}
	else if (clause.getLeftParam().attribute == STMT_NO
		&& clause.getRightParam().attribute == STMT_NO) {
		if (clause.getLeftParam().type != STMT
			&& clause.getRightParam().type != STMT) {
			ClauseResults withClauseResults;
			withClauseResults.instantiateClause(clause);
			vector<vector<int>> withResults;
			withClauseResults.setResults(withResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		}
		else if (clause.getLeftParam().type == STMT) {
			ClauseResults withClauseResults;
			withClauseResults.instantiateClause(clause);
			int type = 0;
			if (clause.getRightParam().type == ASSIGN) type = 1;
			else if (clause.getRightParam().type == WHILE) type = 2;
			else if (clause.getRightParam().type == IF) type = 3;
			else if (clause.getRightParam().type == CALL) type = 4;
			vector<vector<int>> withResults = pkb.getAllStatementsWithType(type);
			vector<vector<int>> newWithResults;
			for (vector<int> i : withResults) {
				newWithResults.push_back({ i[0], i[0] });
			}
			withClauseResults.setResults(newWithResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		}
		else if (clause.getRightParam().type == STMT) {
			ClauseResults withClauseResults;
			withClauseResults.instantiateClause(clause);
			int type = 0;
			if (clause.getLeftParam().type == ASSIGN) type = 1;
			else if (clause.getLeftParam().type == WHILE) type = 2;
			else if (clause.getLeftParam().type == IF) type = 3;
			else if (clause.getLeftParam().type == CALL) type = 4;
			vector<vector<int>> withResults = pkb.getAllStatementsWithType(type);
			vector<vector<int>> newWithResults;
			for (vector<int> i : withResults) {
				newWithResults.push_back({ i[0], i[0] });
			}
			withClauseResults.setResults(newWithResults);
			EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		}
	}
	else if (EvaluatorHelper::withClauseNumSyns(clause, iTable) == 1) {
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
		valueOfParam = pkb.getProcedureId(paramWithValue.value);
	}
	else if (paramInTable.attribute == VARNAME) { // Constant is variable name
		valueOfParam = pkb.getVariableId(paramWithValue.value);
	}
	else { // Constant is integer
		valueOfParam = stoi(paramWithValue.value);
	}

	vector<vector<int>> updatedTable;
	if (paramInTable.type != CALL) { // Normal filtering
		for (vector<int> tableRow : iTable.resultsTable) {
			if (tableRow[paramIndex] == valueOfParam) updatedTable.push_back(tableRow);
		}
		iTable.setResultsTable(updatedTable);
	} else { // CALL case,
		Param clauseCallParam;
		set<string> clauseCallValues;
		if (Utils::isSynonym(clause.getLeftParam())) { // Left param is CALL
			clauseCallParam = clause.getLeftParam();
			clauseCallValues.insert(clause.getRightParam().value);
		} else { // Right param is CALL
			clauseCallParam = clause.getRightParam();
			clauseCallValues.insert(clause.getLeftParam().value);
		}
		handleCallInWithClause(clauseCallParam, clauseCallValues, iTable);
	}
};

/* Filters table for with equating two variables */
void QueryEvaluator::handleWithEquateVariables(Clause &clause, IntermediateTable &iTable) {
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();
	int firstParamTableIndex = iTable.getParamIndex(lhs);
	int secondParamTableIndex = iTable.getParamIndex(rhs);

	if (lhs.type == CONSTANT && rhs.type == CONSTANT) {
		vector<vector<int>> updatedTable;
		if (firstParamTableIndex > -1 && secondParamTableIndex > -1) {
			//both exist in the table and need to be intersected
			for (vector<int> tableRow : iTable.resultsTable) {
				int lhsIntValue = tableRow[firstParamTableIndex];
				int rhsIntValue = tableRow[secondParamTableIndex];

				if (tableRow[firstParamTableIndex] == tableRow[secondParamTableIndex]) { // Compare int value
					updatedTable.push_back(tableRow);
				}
				else { ; }
			}
		}
		else if (firstParamTableIndex == -1) {
			//firstParam does not exist in table
			lhs.attribute = NONE;
			iTable.addTableParams(lhs);
			for (vector<int> tableRow : iTable.resultsTable) {
				int rhsIntValue = tableRow[secondParamTableIndex];
				tableRow.push_back(rhsIntValue);
				updatedTable.push_back(tableRow);
			}
		}
		else if (secondParamTableIndex == -1) {
			//secondParam does not exist in table
			rhs.attribute = NONE;
			iTable.addTableParams(rhs);
			for (vector<int> tableRow : iTable.resultsTable) {
				int lhsIntValue = tableRow[firstParamTableIndex];
				tableRow.push_back(lhsIntValue);
				updatedTable.push_back(tableRow);
			}
		}
		iTable.setResultsTable(updatedTable);
	}
	else if (lhs.type == CONSTANT || rhs.type == CONSTANT) {
		vector<vector<int>> updatedTable;
		if (lhs.type == CONSTANT) {
			if (firstParamTableIndex > -1) { // lhs is in params
				rhs.attribute = NONE;
				iTable.addTableParams(rhs);
				for (vector<int> tableRow : iTable.resultsTable) {
					int lhsIntValue = tableRow[firstParamTableIndex];
					int type = 0;
					if (rhs.type == STMT) type = 0;
					else if (rhs.type == ASSIGN) type = 1;
					else if (rhs.type == WHILE) type = 2;
					else if (rhs.type == IF) type = 3;
					else if (rhs.type == CALL) type = 4;
					if (pkb.checkStatementHasType(lhsIntValue, type)) {
						tableRow.push_back(lhsIntValue);
						updatedTable.push_back(tableRow);
					}
				}
			}
			else if (secondParamTableIndex > -1) { // lhs is in params
				lhs.attribute = NONE;
				iTable.addTableParams(lhs);
				for (vector<int> tableRow : iTable.resultsTable) {
					int rhsIntValue = tableRow[secondParamTableIndex];
					std::vector<std::vector<int>> consts = pkb.getAllConstants();
					bool hasConst = false;
					for (std::vector<int> c : consts) {
						if (c[0] == rhsIntValue) {
							hasConst = true;
							break;
						}
					}
					if (hasConst) {
						tableRow.push_back(rhsIntValue);
						updatedTable.push_back(tableRow);
					}
				}
			}
		}
		else if (rhs.type == CONSTANT) {
			if (secondParamTableIndex > -1) { // lhs is in params
				lhs.attribute = NONE;
				iTable.addTableParams(lhs);
				for (vector<int> tableRow : iTable.resultsTable) {
					int rhsIntValue = tableRow[secondParamTableIndex];
					int type = 0;
					if (lhs.type == STMT) type = 0;
					else if (lhs.type == ASSIGN) type = 1;
					else if (lhs.type == WHILE) type = 2;
					else if (lhs.type == IF) type = 3;
					else if (lhs.type == CALL) type = 4;
					if (pkb.checkStatementHasType(rhsIntValue, type)) {
						tableRow.push_back(rhsIntValue);
						updatedTable.push_back(tableRow);
					}
				}
			}
			else if (firstParamTableIndex > -1) { // lhs is in params
				rhs.attribute = NONE;
				iTable.addTableParams(rhs);
				for (vector<int> tableRow : iTable.resultsTable) {
					int lhsIntValue = tableRow[firstParamTableIndex];
					std::vector<std::vector<int>> consts = pkb.getAllConstants();
					bool hasConst = false;
					for (std::vector<int> c : consts) {
						if (c[0] == lhsIntValue) {
							hasConst = true;
							break;
						}
					}
					if (hasConst) {
						tableRow.push_back(lhsIntValue);
						updatedTable.push_back(tableRow);
					}
				}
			}
		}
		iTable.setResultsTable(updatedTable);
	}
	else if (lhs.type != CALL && rhs.type != CALL) {
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
	}
	else { // CALL
		vector<vector<int>> updatedTable;
		if (firstParamTableIndex > -1 && secondParamTableIndex > -1) {
			//both exist in the table and need to be intersected
			for (vector<int> tableRow : iTable.resultsTable) {
				int lhsIntValue = tableRow[firstParamTableIndex];
				int rhsIntValue = tableRow[secondParamTableIndex];

				if (tableRow[firstParamTableIndex] == tableRow[secondParamTableIndex]) { // Compare int value
					updatedTable.push_back(tableRow);
				}
				else { ; }
			}
		}
		else if (firstParamTableIndex == -1) {
			//firstParam does not exist in table
			lhs.attribute = NONE;
			iTable.addTableParams(lhs);
			for (vector<int> tableRow : iTable.resultsTable) {
				int rhsIntValue = tableRow[secondParamTableIndex];
				tableRow.push_back(rhsIntValue);
				updatedTable.push_back(tableRow);
			}
		}
		else if (secondParamTableIndex == -1) {
			//secondParam does not exist in table
			rhs.attribute = NONE;
			iTable.addTableParams(rhs);
			for (vector<int> tableRow : iTable.resultsTable) {
				int lhsIntValue = tableRow[firstParamTableIndex];
				tableRow.push_back(lhsIntValue);
				updatedTable.push_back(tableRow);
			}
		}
		iTable.setResultsTable(updatedTable);
	}
};

/* Handles with evaluation when not found in table */
bool QueryEvaluator::handleWithEvaluation(Clause &withClause, IntermediateTable &iTable) {
	Param lhs = withClause.getLeftParam();
	Param rhs = withClause.getRightParam();
	ClauseResults withClauseResults;
	vector<vector<int>> withResults;

	withClauseResults.instantiateClause(withClause);
	if (Utils::isSynonym(lhs) || Utils::isSynonym(rhs)) {
		// Both synonyms
		if (Utils::isSynonym(lhs) && Utils::isSynonym(rhs)) {
			set<int> lhsParamSet = getParamSet(lhs);
			set<int> rhsParamSet = getParamSet(rhs);

			Utils::intersectSets(pkb, lhsParamSet, rhsParamSet, lhs, rhs, withResults);

		} else if (Utils::isSynonym(lhs)) { // LHS syn
			set<int> lhsParamSet = getParamSet(lhs);

			for (int value : lhsParamSet) {
				if (value == getId(rhs, lhs.type, lhs.attribute)) {
					if (lhs.type == CALL && lhs.attribute == PROCNAME) {
						std::vector<std::vector<int>> r = pkb.getCallStatementsCallingProcedure(value);
						for (std::vector<int> v : r) {
							withResults.push_back({ v[0] });
						}
					}
					else {
						withResults.push_back({ value });
					}
				}
			}

		} else if (Utils::isSynonym(rhs)) { // RHS syn
			set<int> rhsParamSet = getParamSet(rhs);

			for (int value : rhsParamSet) {
				vector<int> withTableRow;
				if (value == getId(lhs, rhs.type, rhs.attribute)) {
					if (rhs.type == CALL && rhs.attribute == PROCNAME) {
						std::vector<std::vector<int>> r = pkb.getCallStatementsCallingProcedure(value);
						for (std::vector<int> v : r) {
							withResults.push_back({ v[0] });
						}
					}
					else {
						withResults.push_back({ value });
					}
				}
			}
		}

		// Merge directly (No consideration for calls since not in table)
		withClauseResults.setResults(withResults);
		EvaluatorHelper::mergeClauseTable(withClauseResults, iTable);
		if (withResults.size() > 0) {
			return true;
		}
		return false;
	} else {
		return lhs.value == rhs.value;
	}
};

/* Handles Calls in With Clause */
void QueryEvaluator::handleCallInWithClause(Param clauseCallParam, set<string> &clauseCallValues, IntermediateTable &iTable) {

	int paramTableIndex = iTable.getParamIndex(clauseCallParam);
	Param tableCallParam = iTable.getParamFromIndex(paramTableIndex);

	vector<vector<int>> filteredResults;
	// Iterate through possible assignment values
	for (vector<int> tableRow : iTable.resultsTable) {
		for (string callValue : clauseCallValues) {
			int intCallValue = (clauseCallParam.attribute == PROCNAME) ?
				pkb.getProcedureId(callValue) : stoi(callValue);
			if (clauseCallParam.attribute == tableCallParam.attribute) { // Trivial merge
				if (intCallValue == tableRow[paramTableIndex]) {
					filteredResults.push_back(tableRow);
				}
			} else if (clauseCallParam.attribute != PROCNAME) { // clause PROG_LINE, table STMT_NO

				/* Get all statements numbers from called procedure in row and check equality with clause
				program line. Replaces p.procname with p.prog_line (more limiting) */
				vector<vector<int>> progLines = pkb.getCallStatementsCallingProcedure(tableRow[paramTableIndex]);
				for (vector<int> progLine : progLines) {
					if (progLine[0] == intCallValue) {
						tableRow[paramTableIndex] = intCallValue;
						filteredResults.push_back(tableRow);
					}
				}

			} else { // clause PROCNAME, table STMT_NO
				vector<vector<int>> progLines = pkb.getCallStatementsCallingProcedure(intCallValue);
				for (vector<int> progLine : progLines) {
					if (progLine[0] == tableRow[paramTableIndex]) {
						filteredResults.push_back(tableRow);
					}
				}
			}
		}
	}

	/* replace tables CALL.PROCNAME with CALL.PROG_LINE params */
	if (clauseCallParam.attribute != PROCNAME && tableCallParam.attribute == PROCNAME) {
		iTable.replaceTableParam(tableCallParam, clauseCallParam);
	}
	iTable.setResultsTable(filteredResults);

};

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
				Param currentParam = selectedParams[j];
				if (currentParam.type == PROCEDURE) {
					value = pkb.getProcedureName(paramValue);
				} else if (currentParam.type == VARIABLE) {
					value = pkb.getVariableName(paramValue);
				} else if (currentParam.type == CALL) { // Special case with calls
					if (currentParam.attribute == PROCNAME) { // Get procname
						if (mergedTable.getParamAttr(currentParam) == PROCNAME) { // Same attr in table
							value = pkb.getProcedureName(paramValue);
						} else { // Calls line num in table, get procname from line
							int procId = pkb.getProcedureCalledByCallStatement(paramValue)[0][0];
							value = pkb.getProcedureName(procId);
						}
					} else { // Get line number
						value = to_string(paramValue);
					}
				} else { // Default case just convert int to string
					value = to_string(paramValue);
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
			else if (p.type == CALL && p.attribute == PROCNAME) {
				std::vector<std::vector<int>> px = pkb.getProcedureCalledByCallStatement(tableRow[paramInt]);
				paramVal = pkb.getProcedureName(px[0][0]);
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

/* Obtains the universe set for cross of two params */
vector<vector<int>> QueryEvaluator::universeSet(Param lhs, Param rhs) {
	vector<vector<int>> lhsSet = setValuesforParam(lhs);
	vector<vector<int>> rhsSet = setValuesforParam(rhs);
	return EvaluatorHelper::crossVectors(lhsSet, rhsSet);
};

/* Obtains the universe set for a single param */
vector<vector<int>> QueryEvaluator::setValuesforParam(Param p) {
	if (p.type == STMT) {
		return pkb.getAllStatements();
	} else if (p.type == ASSIGN) {
		return pkb.getAllStatementsWithType(1);
	} else if (p.type == WHILE) {
		return pkb.getAllStatementsWithType(2);
	} else if (p.type == IF) {
		return pkb.getAllStatementsWithType(3);
	} else if (p.type == CALL) {
		return pkb.getAllStatementsWithType(4);
	} else if (p.type == VARIABLE) { // Variable IDs
		return pkb.getAllVariables();
	} else if (p.type == CONSTANT) {
		return pkb.getAllConstants();
	} else if (p.type == PROCEDURE) { // Procedure IDs
		return pkb.getAllProcedures();
	}
};
