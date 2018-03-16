#pragma once

#include <string>

using namespace std;

enum ParamType {
	ASSIGN, STMT, STMTLST, WHILE, PROG_LINE, IF, CALL, //Synonym with statement number
	VARIABLE, CONSTANT, //Synonym
	PROCEDURE, //Synonym
	INTEGER,  //Statement Number
	EXPR, //For pattern assign (RHS)
	PROCNAME, VARNAME, VALUE, STMT_NO, //attrName for with clause
	IDENT, //IDENT refers to "x"
	SYNONYM, //Synonym is the general term
	BOOLEAN, //return true/false
	ALL
}; // ALL represents _

enum AttrType {
	NONE,
	VALUE,
	VARNAME,
	PROCNAME,
	STMTNO,
};

struct Param {
	ParamType type;
	string value;
	AttrType dotType;
};
