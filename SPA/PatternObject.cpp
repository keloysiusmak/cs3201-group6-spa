#pragma once

using namespace std;

#include "PatternObject.h"

int LHS, LHS_type, RHS, RHS_type;

PatternObject::PatternObject(int LHSid, int LHStype, int RHSid, int RHStype) {
	LHS = LHSid;
	LHS_type = LHStype;
	RHS = RHSid;
	RHS_type = RHStype;
}

int PatternObject::getLHSid() {
	return LHS;
}

int PatternObject::getLHStype() {
	return LHS_type;
}

int PatternObject::getRHSid() {
	return RHS;
}

int PatternObject::getPHStype() {
	return RHS_type;
}