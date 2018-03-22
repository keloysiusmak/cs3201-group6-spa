#pragma once

#include <string>

using namespace std;

enum ParamType {
	ASSIGN, STMT, STMTLST, WHILE, PROG_LINE, IF, CALL, //Synonym with statement number
	VARIABLE, CONSTANT, //Synonym
	PROCEDURE, //Synonym
	INTEGER,  //Statement Number
	EXPR, EXPR_EXACT, //For pattern assign (RHS)
	IDENT, //IDENT refers to "x"
	SYNONYM, //Synonym is the general term
	BOOLEAN, //return true/false
	PROC_IDENT, VAR_IDENT, //differentiate IDENT
	ALL
}; // ALL represents _


/* Attribute type to account for with statements */
enum AttrType {
	PROCNAME, VARNAME, VALUE, STMT_NO, NONE //attrName for with clause
};

struct Param {
	ParamType type;
	string value;
	AttrType attribute;
};
