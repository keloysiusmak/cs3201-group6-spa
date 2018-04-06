#pragma once

#include <string>
#include "../SPA/Constants.h"

using namespace std;

struct Param {
	ParamType type;
	string value;
	AttrType attribute;

	// For usage of Param as key of map
	bool operator<(const Param &other) const {
		return value < other.value;
	}
};

