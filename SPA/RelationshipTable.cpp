#include "RelationshipTable.h"

RelationshipTable::RelationshipTable() {
	unordered_set<ParamType> leftArg, rightArg;

	//Uses & Modifies
	leftArg = { STMT, ASSIGN, WHILE, PROG_LINE, IF, PROCEDURE, CALL, PROC_IDENT, INTEGER };
	rightArg = { VARIABLE, VAR_IDENT, ALL };
	Relationship rel1(2, leftArg, rightArg);
	relTable["Uses"] = rel1;
	relTable["Modifies"] = rel1;
	leftArg.clear();
	rightArg.clear();

	//Parent & Parent*
	leftArg = { STMT, WHILE, PROG_LINE, IF, INTEGER, ALL };
	rightArg = { STMT, ASSIGN, WHILE, PROG_LINE, IF, CALL, INTEGER, ALL };
	Relationship rel2(2, leftArg, rightArg);
	relTable["Parent"] = rel2;
	relTable["ParentT"] = rel2;
	leftArg.clear();
	rightArg.clear();

	//Follows & Follows*
	leftArg = { STMT, ASSIGN, WHILE, PROG_LINE, IF, CALL, INTEGER, ALL };
	rightArg = { STMT, ASSIGN, WHILE, PROG_LINE, IF, CALL, INTEGER, ALL };
	Relationship rel3(2, leftArg, rightArg);
	relTable["Follows"] = rel3;
	relTable["FollowsT"] = rel3;
	leftArg.clear();
	rightArg.clear();

	//Next & Next*
	leftArg = { STMT, ASSIGN, WHILE, IF, CALL, PROG_LINE, INTEGER, ALL };
	rightArg = { STMT, ASSIGN, WHILE, IF, CALL, PROG_LINE, INTEGER, ALL };
	Relationship rel4(2, leftArg, rightArg);
	relTable["Next"] = rel4;
	relTable["NextT"] = rel4;
	leftArg.clear();
	rightArg.clear();

	//Calls & Calls*
	leftArg = { PROCEDURE, PROC_IDENT, ALL };
	rightArg = { PROCEDURE, PROC_IDENT, ALL };
	Relationship rel5(2, leftArg, rightArg);
	relTable["Calls"] = rel5;
	relTable["CallsT"] = rel5;
	leftArg.clear();
	rightArg.clear();

	//Affects & Affects*
	leftArg = { PROG_LINE, STMT, ASSIGN, INTEGER, ALL };
	rightArg = { PROG_LINE, STMT, ASSIGN, INTEGER, ALL };
	Relationship rel6(2, leftArg, rightArg);
	relTable["Affects"] = rel6;
	relTable["AffectsT"] = rel6;
	leftArg.clear();
	rightArg.clear();

	//Pattern assign
	leftArg = { VARIABLE, VAR_IDENT, ALL };
	rightArg = { EXPR, EXPR_EXACT, ALL };
	Relationship rel7(2, leftArg, rightArg);
	relTable["assignpt"] = rel7;
	leftArg.clear();
	rightArg.clear();

	//Pattern if & Pattern while
	leftArg = { VARIABLE, VAR_IDENT, ALL };
	rightArg = { ALL };
	Relationship rel8(3, leftArg, rightArg);
	Relationship rel9(2, leftArg, rightArg);
	relTable["ifpt"] = rel8;
	relTable["whilept"] = rel9;
	leftArg.clear();
	rightArg.clear();

	//With Number
	leftArg = { STMT, ASSIGN, WHILE, IF, CALL, CONSTANT, PROG_LINE, INTEGER };
	rightArg = { STMT, ASSIGN, WHILE, IF, CALL, CONSTANT, PROG_LINE, INTEGER };
	Relationship rel10(2, leftArg, rightArg);
	relTable["withNumber"] = rel10;
	leftArg.clear();
	rightArg.clear();

	//With String
	leftArg = { PROCEDURE, CALL, VARIABLE, VAR_IDENT, PROC_IDENT };
	rightArg = { PROCEDURE, CALL, VARIABLE, VAR_IDENT, PROC_IDENT };
	Relationship rel11(2, leftArg, rightArg);
	relTable["withString"] = rel11;
	leftArg.clear();
	rightArg.clear();
}

bool RelationshipTable::isValidRelationship(string relRef) {
	return relTable.find(relRef) != relTable.end();
}

bool RelationshipTable::isValidNumOfArgs(string relRef, int num) {
	return relTable.find(relRef)->second.getNumOfArguments() == num;
}

bool RelationshipTable::isValidLeftArg(string relRef, ParamType leftArgType) {

	auto rel = relTable.find(relRef)->second;
	unordered_set<ParamType> getLeftArgSet = rel.getLeftArg();
	return getLeftArgSet.find(leftArgType) != getLeftArgSet.end();
}

bool RelationshipTable::isValidRightArg(string relRef, ParamType rightArgType) {

	auto rel = relTable.find(relRef)->second;
	unordered_set<ParamType> getRightArgSet = rel.getRightArg();
	return getRightArgSet.find(rightArgType) != getRightArgSet.end();
}

bool RelationshipTable::isValidArg(string relRef, ParamType leftArgType, ParamType rightArgType) {
	return isValidLeftArg(relRef, leftArgType) && isValidRightArg(relRef, rightArgType);
}