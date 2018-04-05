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
	t = WITHCLAUSE;
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