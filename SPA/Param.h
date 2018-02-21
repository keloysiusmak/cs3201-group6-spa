#pragma once

#include <string>

using namespace std;

enum ParamType { assign, stmt, variable, whiles, prog_line, 
				integer, constant, var_name, 
				ident, synonym, all };

struct Param {
	ParamType type;
	string value;
};
