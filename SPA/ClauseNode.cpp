#include "ClauseNode.h"


ClauseNode::ClauseNode()
{

}

ClauseNode::ClauseNode(Clause clause)
{
	c = clause;
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