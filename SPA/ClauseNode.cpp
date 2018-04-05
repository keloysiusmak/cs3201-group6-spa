#include "ClauseNode.h"


ClauseNode::ClauseNode()
{

}

ClauseNode::ClauseNode(Clause clause, CLAUSE_NODE_TYPE t)
{
	if (t == WITH_CLAUSE) {
		w = clause;
	}
	else {
		c = clause;
	}
}

ClauseNode::ClauseNode(Pattern pattern)
{
	p = pattern;
}

ClauseNode::ClauseNode(OPERATORS op)
{
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