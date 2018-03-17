#pragma once

#include "QueryObject.h"

using namespace std;


QueryObject::QueryObject() {

}

QueryObject::QueryObject(string query) {

}

void QueryObject::insertClause(RelRef relRef, ParamType firstParamType, string firstParam, ParamType secondParamType, string secondParam) {

	Param fp;
	fp.type = firstParamType;
	fp.value = firstParam;
	fp.attribute = NONE;

	Param sp;
	sp.type = secondParamType;
	sp.value = secondParam;
	sp.attribute = NONE;

	Clause clause(relRef, fp, sp);

	_clauses.push_back(clause);
}

void QueryObject::insertPattern(ParamType entityType, string entity, ParamType leftParamType, string leftParam, ParamType rightParamType, string rightParam) {

	Param ep;
	ep.type = entityType;
	ep.value = entity;
	ep.attribute = NONE;

	Param lp;
	lp.type = leftParamType;
	lp.value = leftParam;
	lp.attribute = NONE;

	Param rp;
	rp.type = rightParamType;
	rp.value = rightParam;
	rp.attribute = NONE;

	Pattern pattern(ep, lp, rp);

	_Patterns.push_back(pattern);
}

void QueryObject::insertSelectStmt(ParamType selectType, string synonym, AttrType attr) {
	Param result;
	result.type = selectType;
	result.value = synonym;
	result.attribute = attr;

	_selectStmt.push_back(result);
}


void QueryObject::updateWithClause(ParamType attrRef_AttrName, string attrRefSynonym, ParamType ref_AttrName, string ref) {

}

vector<Clause> QueryObject::getClauses() {
	return _clauses;
}

vector<Pattern> QueryObject::getPatterns() {
	return _Patterns;
}

vector<Clause> QueryObject::getWithClauses() {
	return _withClauses;
}

vector<Param> QueryObject::getSelectStatements() {
	return _selectStmt;
}
