#pragma once

#include <string>
#include "../SPA/Constants.h"

using namespace std;

/* Attribute type to account for with statements */
enum AttrType {
	PROCNAME, VARNAME, VALUE, STMT_NO, NONE //attrName for with clause
};

struct Param {
	ParamType type;
	string value;
	AttrType attribute;

	// For usage of Param as key of map
	bool operator<(const Param &other) const {
		return value < other.value;
	}
};

