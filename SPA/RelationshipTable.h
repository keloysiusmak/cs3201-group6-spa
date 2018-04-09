#pragma once
#include <string>
#include <unordered_map>
#include <regex>
#include "Relationship.h"

using namespace std;

class RelationshipTable {
private:
	unordered_map<string, Relationship> relTable;
public:
	RelationshipTable();
	bool isValidRelationship(string relRef);
	bool isValidNumOfArgs(string relRef, int num);
	bool isValidLeftArg(string relRef, ParamType leftArgType);
	bool isValidRightArg(string relRef, ParamType rightArgType);
	bool isValidArg(string relRef, ParamType leftArgType, ParamType rightArgType);
};
