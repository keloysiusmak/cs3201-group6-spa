#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::tr1;

class Pattern {
public:
	std::string LHS;
	std::string LHS_type;
	std::string RHS;
	std::string RHS_type;
};