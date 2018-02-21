#pragma once

#include <string>

using namespace std;

enum ParamType { ASSIGN, STMT, VARIABLE, WHILE, PROG_LINE, 
				INTEGER, CONSTANT, VAR_NAME, 
				IDENT, SYNONYM, ALL };

struct Param {
	ParamType type;
	string value;
};
