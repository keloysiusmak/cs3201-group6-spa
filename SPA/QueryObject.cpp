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
	Param result;
	result.type = selectType;
	result.value = synonym;

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

vector<Param> QueryObject::getSelectStatements() {
	return _selectStmt;
}
