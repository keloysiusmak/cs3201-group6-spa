#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Pattern.h"

using namespace std::tr1;

class PKB {
public:
	/*  PKB Structure*/
	unordered_map<int, std::vector<std::vector<int>>> tables[7];
	unordered_map<int, std::vector<std::vector<std::string>>> nameTables[2];

	/* PKB Operations */
	bool insertToTable(int, int, std::vector<std::vector<int>>);
	bool insertToNameTable(int, int, std::vector<std::vector<std::string>>);
	std::vector<std::vector<int>> getFromTable(int, int);
	std::vector<std::vector<std::string>> getFromNameTable(int, int);

	/* Accessor Operations */
	std::string getProcedureName(int);
	std::string getVariableName(int);
	int getProcedureId(std::string);
	int getVariableId(std::string);

	/* Follows Operations */
	int getFollowsBefore(int);
	int getFollowsAfter(int);
	std::vector<int> getFollowsBeforeStar(int);
	std::vector<int> getFollowsAfterStar(int);
	bool checkFollows(int, int);
	bool checkFollowsStar(int, int);
	std::vector<std::vector<int>> getAllFollows();
	unordered_map<int, std::vector<int>> getAllFollowsStar();

	/* Parent Operations */
	int getParent(int);
	std::vector<int> getParentStar(int);
	std::vector<int> getChildren(int);
	std::vector<int> getChildrenStar(int);
	bool checkParent(int, int);
	bool checkParentStar(int, int);
	std::vector<std::vector<int>> getAllParent();
	unordered_map<int, std::vector<int>> getAllParentStar();

	/* Uses Operations */
	std::vector<int> getUsesVariablesFromStatement(int);
	std::vector<int> getStatementFromUsesVariables(int);
	unordered_map<int, std::vector<int>> getAllStatementUsesVariables();
	unordered_map<int, std::vector<int>> getAllUsesVariableStatements();
	bool checkStatementUsesVariable(int, int);

	/* Modifies Operations */
	std::vector<int> getModifiesVariablesFromStatement(int);
	std::vector<int> getStatementFromModifiesVariables(int);
	unordered_map<int, std::vector<int>> getAllStatementModifiesVariables();
	unordered_map<int, std::vector<int>> getAllModifiesVariableStatements();
	bool checkStatementModifiesVariable(int, int);

	/* Pattern Operations */
	std::vector<int> getStatementsWithPattern(Pattern);
	bool checkStatementsWithPattern(Pattern);

};