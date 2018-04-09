#include "QueryContent.h"

void QueryContent::insertSelect(ParamType selectType, string synonym, AttrType attr) {
	Param result;
	result.type = selectType;
	result.value = synonym;
	result.attribute = attr;

	_selectStmt.push_back(result);
}
void QueryContent::insertClause(RelRef relRef, ParamType firstParamType, string firstParam, ParamType secondParamType, string secondParam, bool invert) {
	Param fp;
	fp.type = firstParamType;
	fp.value = firstParam;
	fp.attribute = NONE;

	Param sp;
	sp.type = secondParamType;
	sp.value = secondParam;
	sp.attribute = NONE;

	Clause clause(relRef, fp, sp, invert);

	ClauseNode cn;
	cn.setClauseNode(clause);

	_clauses.push_back(cn);
}
void QueryContent::insertPattern(ParamType entityType, string entity, ParamType leftParamType, string leftParam, ParamType rightParamType, string rightParam, bool invert) {
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

	Pattern pattern(ep, lp, rp, invert);

	ClauseNode cn;
	cn.setPatternNode(pattern);

	_patterns.push_back(cn);
}
void QueryContent::insertWithClause(ParamType leftArgType, string leftArg, AttrType leftAttr,
	ParamType rightArgType, string rightArg, AttrType rightAttr, bool invert) {
	
	Param leftParam;
	leftParam.type = leftArgType;
	leftParam.value = leftArg;
	leftParam.attribute = leftAttr;

	Param rightParam;
	rightParam.type = rightArgType;
	rightParam.value = rightArg;
	rightParam.attribute = rightAttr;

	Clause withClause(With, leftParam, rightParam, invert);

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
void QueryContent::setChildren(int vqc) {
	children.push_back(vqc);
}
vector<Param> QueryContent::getSelect() {
	return _selectStmt;
}

vector<ClauseNode> QueryContent::getClauses() {
	return _clauses;
}

vector<ClauseNode> QueryContent::getPattern() {
	return _patterns;
}

vector<ClauseNode> QueryContent::getWithClauses() {
	return _withClauses;
}

void QueryContent::setNegation(CLAUSE_NODE_TYPE nodeRef, int nodePosition) {
	switch (nodeRef) {
	case CLAUSE: {
		ClauseNode& clauseNode = _clauses.at(nodePosition);
		if (clauseNode.getClauseNodeType() == OPERATOR) {
			if (clauseNode.getOperators() == AND) {
				clauseNode.setOperatorNode(OR);
			}
			else {
				clauseNode.setOperatorNode(AND);
			}
		}
		else {
			clauseNode.setNegation(nodeRef);
		}
	}
	break;
	case PATTERN: {
		ClauseNode& patternNode = _patterns.at(nodePosition);
		if (patternNode.getClauseNodeType() == OPERATOR) {
			if (patternNode.getOperators() == AND) {
				patternNode.setOperatorNode(OR);
			}
			else {
				patternNode.setOperatorNode(AND);
			}
		}
		else {
			patternNode.setNegation(nodeRef);
		}
	}
	break;
	case WITH_CLAUSE: {
		ClauseNode& withClauseNode = _withClauses.at(nodePosition);
		if (withClauseNode.getClauseNodeType() == OPERATOR) {
			if (withClauseNode.getOperators() == AND) {
				withClauseNode.setOperatorNode(OR);
			}
			else {
				withClauseNode.setOperatorNode(AND);
			}
		}
		else {
			withClauseNode.setNegation(nodeRef);
		}
	}
	break;
	}
}

vector<int> QueryContent::getChildren() {
	return children;
}
void QueryContent::setClause(vector<ClauseNode> c) {
	_clauses = c;
}
void QueryContent::setWithClause(vector<ClauseNode> w) {
	_withClauses = w;
}
void QueryContent::setPattern(vector<ClauseNode> p) {
	_patterns = p;
}
