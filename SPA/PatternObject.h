#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::tr1;

class PatternObject {
public:
	int LHS, LHS_type, RHS, RHS_type;

	PatternObject(int, int, int, int);
};