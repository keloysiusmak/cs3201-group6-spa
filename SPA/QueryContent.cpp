#include "QueryContent.h"

void QueryContent::insertSelect(ParamType selectType, string synonym, AttrType attr) {
	Param result;
	result.type = selectType;
	result.value = synonym;
	result.attribute = attr;

	_selectStmt.push_back(result);
}
void QueryContent::insertClause(RelRef relRef, ParamType firstParamType, string firstParam, ParamType secondParamType, string secondParam) {
	Param fp;
	fp.type = firstParamType;
	fp.value = firstParam;
	fp.attribute = NONE;

	Param sp;
	sp.type = secondParamType;
	sp.value = secondParam;
	sp.attribute = NONE;

	Clause clause(relRef, fp, sp);

	ClauseNode cn;
	cn.setClauseNode(clause);

	_clauses.push_back(cn);
}
void QueryContent::insertPattern(ParamType entityType, string entity, ParamType leftParamType, string leftParam, ParamType rightParamType, string rightParam) {
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

	ClauseNode cn;
	cn.setPatternNode(pattern);

	_patterns.push_back(cn);
}
void QueryContent::insertWithClause(ParamType leftArgType, string leftArg, AttrType leftAttr,
	ParamType rightArgType, string rightArg, AttrType rightAttr) {
	
	Param leftParam;
	leftParam.type = leftArgType;
	leftParam.value = leftArg;
	leftParam.attribute = leftAttr;

	Param rightParam;
	rightParam.type = rightArgType;
	rightParam.value = rightArg;
	rightParam.attribute = rightAttr;

	Clause withClause(With, leftParam, rightParam);

	ClauseNode cn;
	cn.setWithClauseNode(withClause);

	_withClauses.push_back(cn);
}

void QueryContent::insertOperator(CLAUSE_NODE_TYPE nodeRef, OPERATORS op) {
	switch (nodeRef) {
	case CLAUSE:
		_clauses.push_back(ClauseNode(op));
	break;
	case PATTERN:
		_patterns.push_back(ClauseNode(op));
	break;
	case WITH_CLAUSE:
		_withClauses.push_back(ClauseNode(op));
	break;
	}
}

vector<Param> QueryContent::getSelect() {
	return _selectStmt;
}

vector<ClauseNode> QueryContent::getClauses() {
	return _withClauses;
}

vector<ClauseNode> QueryContent::getPattern() {
	return _patterns;
}

vector<ClauseNode> QueryContent::getWithClauses() {
	return _clauses;
}
