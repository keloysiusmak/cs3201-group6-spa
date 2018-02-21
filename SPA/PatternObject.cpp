#pragma once

using namespace std;

#include "../source/PatternObject.h"

int LHS, LHS_type, RHS, RHS_type;

PatternObject::PatternObject(int LHSid, int LHStype, int RHSid, int RHStype) {
	LHS = LHSid;
	LHS_type = LHStype;
	RHS = RHSid;
	RHS_type = RHStype;
}