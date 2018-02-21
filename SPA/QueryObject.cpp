#pragma once

#include "QueryObject.h"

using namespace std;


QueryObject::QueryObject() {

}

QueryObject::QueryObject(string query) {

}

void QueryObject::insertClause(string relRef, ParamType firstParamType, string firstParam, ParamType secondParamType, string secondParam) {
	
	Param fp;
	fp.type = firstParamType;
	fp.value = firstParam;
	
	Param sp;
	sp.type = secondParamType;
	sp.value = secondParam;

	Clause clause(relRef, fp, sp);

	_clauses.push_back(clause);
}

void QueryObject::insertPattern(ParamType entityType, string entity, ParamType leftParamType, string leftParam, ParamType rightParamType, string rightParam) {

	Param ep;
	ep.type = entityType;
	ep.value = entity;

	Param lp;
	lp.type = leftParamType;
	lp.value = leftParam;

	Param rp;
	rp.type = rightParamType;
	rp.value = rightParam;

	Pattern pattern(ep, lp, rp);

	_Patterns.push_back(pattern);
}

void QueryObject::insertSelectStmt(ParamType selectType, string synonym) {
	_selectStmt.type = selectType;
	_selectStmt.value = synonym;
}

vector<Clause> QueryObject::getClauses() {
	return _clauses;
}

vector<Pattern> QueryObject::getPatterns() {
	return _Patterns;
}

Param QueryObject::getSelectStatement() {
	return _selectStmt;
}
