#pragma once

#include <string>

using namespace std;

enum ParamType { ASSIGN, STMT, STMTLST, WHILE, PROG_LINE, IF, CALL, //Synonym with statement number
				VARIABLE, CONSTANT, //Synonym
				PROCEDURE, //Synonym
				INTEGER,  //Statement Number
				CONST_VALUE, VAR_NAME, //For pattern assign (RHS)
				IDENT, //IDENT refers to "x"
				SYNONYM, //Synonym is the general term
				BOOLEAN, //return true/false
				ALL }; // ALL represents _

struct Param {
	ParamType type;
	string value;
};
