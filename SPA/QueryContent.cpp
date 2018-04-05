#include "QueryContent.h"

void QueryContent::setSelect(vector<Param> p) {
	_selectStmt = p;
}
void QueryContent::insertClauses(ClauseNode c) {
	_clauses.push_back(c);
}
void QueryContent::insertPatterns(ClauseNode c) {
	_clauses.push_back(c);
}
void QueryContent::insertWithClauses(ClauseNode c) {
	_clauses.push_back(c);
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
