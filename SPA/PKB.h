#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../SPA/Pattern.h"
#include "../SPA/Constants.h"

using namespace std::tr1;

class PKB {
private:
	/*  PKB Structure*/
	unordered_map<int, std::vector<std::vector<int>>> tables[23];
	unordered_map<int, std::vector<std::string>> nameTables[3];
	unordered_map<int, unordered_map<int, unordered_map<int, int>>> resultTables;

public:
	/* Parser/DE Operations */
	bool insertToTable(int, int, std::vector<std::vector<int>>);
	int insertToNameTable(int, std::vector<std::string>);
	bool insertToResultTable(Relations, int, int, int);
	std::vector<std::vector<int>> getFromTable(int, int);
	std::vector<std::string> getFromNameTable(int, int);
	int getFromResultTable(Relations, int, int);
	std::string getProcedureName(int);
	std::string getVariableName(int);
	int getProcedureId(std::string);
	int getVariableId(std::string);
	
	/* General Operations */
	std::vector<std::vector<int>> getAllVariables();
	std::vector<std::vector<int>> getAllStatements();
	std::vector<std::vector<int>> getAllStatementsWithType(int);
	std::vector<std::vector<int>> getAllProcedures();
	std::vector<std::vector<int>> getAllStatementsFromProcedure(int);
	std::vector<std::vector<int>> getProcedureFromStatement(int);
	bool checkStatementHasType(int, int);

	/* Statement List Operations */
	std::vector<std::vector<int>> getAllStatementLists();
	std::vector<std::vector<int>> getAllStatementListsFirstStmt();

	/* Optimization Operations */
	std::vector<std::vector<int>> getWithProcNameVarName();
	std::vector<std::vector<int>> getWithProcNameCallProcName();
	std::vector<std::vector<int>> getWithVarNameCallProcName();
	std::vector<std::vector<int>> getWithStmtNoConstValue(int);
	std::vector<std::vector<int>> getPatternOneSyn(TYPES);
	std::vector<std::vector<int>> getPatternTwoSyn(TYPES);

	/* Follows Operations */
	std::vector<std::vector<int>> getFollowsBefore(int);
	std::vector<std::vector<int>> getFollowsAfter(int);
	std::vector<std::vector<int>> getFollowsBeforeStar(int);
	std::vector<std::vector<int>> getFollowsAfterStar(int);
	std::vector<std::vector<int>> getAllFollows();
	std::vector<std::vector<int>> getAllFollowsStar();
	bool checkFollows(int, int);
	bool checkFollowsStar(int, int);

	/* Parent Operations */
	std::vector<std::vector<int>> getParent(int);
	std::vector<std::vector<int>> getParentStar(int);
	std::vector<std::vector<int>> getChildren(int);
	std::vector<std::vector<int>> getChildrenStar(int);
	bool checkParent(int, int);
	bool checkParentStar(int, int);
	std::vector<std::vector<int>> getAllParent();
	std::vector<std::vector<int>> getAllParentStar();

	/* Uses Operations */
	std::vector<std::vector<int>> getUsesVariablesFromStatement(int);
	std::vector<std::vector<int>> getStatementsFromUsesVariable(int);
	std::vector<std::vector<int>> getUsesVariablesFromProcedure(int);
	std::vector<std::vector<int>> getProceduresFromUsesVariable(int);
	std::vector<std::vector<int>> getAllStatementUsesVariables();
	std::vector<std::vector<int>> getAllVariableUsesStatements();
	std::vector<std::vector<int>> getAllProcedureUsesVariables();
	std::vector<std::vector<int>> getAllVariableUsesProcedures();
	bool checkStatementUsesVariable(int, int);
	bool checkProcedureUsesVariable(int, int);

	/* Modifies Operations */
	std::vector<std::vector<int>> getModifiesVariablesFromStatement(int);
	std::vector<std::vector<int>> getStatementsFromModifiesVariable(int);
	std::vector<std::vector<int>> getModifiesVariablesFromProcedure(int);
	std::vector<std::vector<int>> getProceduresFromModifiesVariable(int);
	std::vector<std::vector<int>> getAllStatementModifiesVariables();
	std::vector<std::vector<int>> getAllVariableModifiesStatements();
	std::vector<std::vector<int>> getAllProcedureModifiesVariables();
	std::vector<std::vector<int>> getAllVariableModifiesProcedures();
	bool checkStatementModifiesVariable(int, int);
	bool checkProcedureModifiesVariable(int, int);

	/* Next Operations */
	std::vector<std::vector<int>> getNextBefore(int);
	std::vector<std::vector<int>> getNextAfter(int);
	std::vector<std::vector<int>> getNextBeforeStar(int);
	std::vector<std::vector<int>> getNextAfterStar(int);
	std::vector<std::vector<int>> getAllNext();
	std::vector<std::vector<int>> getAllNextStar();
	bool checkNext(int, int);
	bool checkNextStar(int, int);

	/* Calls Operations */
	std::vector<std::vector<int>> getCallsBefore(int);
	std::vector<std::vector<int>> getCallsAfter(int);
	std::vector<std::vector<int>> getCallsBeforeStar(int);
	std::vector<std::vector<int>> getCallsAfterStar(int);
	std::vector<std::vector<int>> getAllCalls();
	std::vector<std::vector<int>> getAllCallsStar();
	bool checkCalls(int, int);
	bool checkCallsStar(int, int);

	/* Call Statement Operations */
	std::vector<std::vector<int>> getCallStatementsCallingProcedure(int);
	std::vector<std::vector<int>> getProcedureCalledByCallStatement(int);
	std::vector<std::vector<int>> getAllCallStatementsCallingProcedure();
	std::vector<std::vector<int>> getAllProcedureCalledByCallStatement();

	/* Pattern Operations */
	std::vector<std::vector<int>> getStatementsWithPattern(Pattern);

	/* Affects Operations */
	std::vector<std::vector<int>> getAffectsBefore(int);
	std::vector<std::vector<int>> getAffectsAfter(int);
	std::vector<std::vector<int>> getAffectsBeforeStar(int);
	std::vector<std::vector<int>> getAffectsAfterStar(int);
	std::vector<int> PKB::processAffectsOrder(int);
	bool checkAffects(int, int);
	bool checkAffectsStar(int, int);
	std::vector<std::vector<int>> getAllAffects();
	std::vector<std::vector<int>> getAllAffectsStar();

	/* Constant Operations */
	std::vector<std::vector<int>> getStatementsWithConstant(int);
	std::vector<std::vector<int>> getProceduresWithConstant(int);
	std::vector<std::vector<int>> getAllConstantsFromProcedure(int);
	std::vector<std::vector<int>> getAllConstantsFromStatement(int);
	std::vector<std::vector<int>> getAllConstants();

};