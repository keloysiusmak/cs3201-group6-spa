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
	c = withClause;
}

void ClauseNode::setOperatorNode(OPERATORS op) {
	t = OPERATOR;
	o = op;
}

void ClauseNode::addChildren(ClauseNode c) {
	children.push_back(c);
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