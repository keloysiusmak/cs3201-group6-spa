#include "ClauseNode.h"


ClauseNode::ClauseNode()
{

}

ClauseNode::ClauseNode(OPERATORS op)
{
	t = OPERATOR;
	o = op;
}

void ClauseNode::setClauseNode(Clause clause) {
	t = CLAUSE;
	c = clause;
}

void ClauseNode::setPatternNode(Pattern pattern) {
	t = PATTERN;
	p = pattern;
}

void ClauseNode::setWithClauseNode(Clause withClause) {
	t = WITH_CLAUSE;
	w = withClause;
}

void ClauseNode::setOperatorNode(OPERATORS op) {
	t = OPERATOR;
	o = op;
}

void ClauseNode::addChildren(ClauseNode c) {
	children.push_back(c);
}

void ClauseNode::setNegation(CLAUSE_NODE_TYPE nodeType) {
	switch (nodeType) {
	case CLAUSE:
		c.setInverted();
		break;
	case PATTERN:
		p.setInverted();
		break;
	case WITH_CLAUSE:
		w.setInverted();
		break;
	}
}

vector<ClauseNode> ClauseNode::getChildren() {
	return children;
}
CLAUSE_NODE_TYPE ClauseNode::getClauseNodeType() {
	return t;
}
Clause ClauseNode::getClause() {
	return c;
}
Clause ClauseNode::getWithClause() {
	return w;
}
Pattern ClauseNode::getPattern() {
	return p;
}
OPERATORS ClauseNode::getOperators() {
	return o;
}