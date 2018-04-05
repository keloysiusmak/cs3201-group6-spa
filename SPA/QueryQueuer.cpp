#include "QueryQueuer.h"

using namespace std;

QueryQueuer::QueryQueuer() {
	bool validQuery = true;
	QueryContent qc;
}

void QueryQueuer::setEvaluator(QueryEvaluator &evaluator) {
	_evaluator = evaluator;
}

bool QueryQueuer::isValidQuery() {
	return validQuery;
}

void QueryQueuer::setInvalidQuery(string message) {
	validQuery = false;
	if (message != "") invalidQueryMessage = { message };
	else invalidQueryMessage = {};
};

void QueryQueuer::setQueryContent(QueryContent setQc) {
	qc = setQc;
}
QueryContent QueryQueuer::getQueryContent() {
	return qc;
}

std::vector<QueryObject> QueryQueuer::parseQueryContent() {

	std::vector<QueryObject> queryObjectArray;
	std::vector<ClauseNode> clauses;
	std::vector<ClauseNode> withClauses;
	std::vector<ClauseNode> pattern;

	std::vector<ClauseNode> clausesNodes = qc.getClauses();
	if (clausesNodes.size() > 0) {
		std::vector<ClauseNode> temp;
		for (int i = 0; i < clausesNodes.size(); i++) {
			if (clausesNodes[i].getClauseNodeType() == OPERATOR) {
				if (clausesNodes[i].getOperators() == AND) {
					if (temp.size() == 0) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(clauses[0]);
						newParent.addChildren(clauses[1]);
						clauses.clear();
						clauses.push_back(newParent);
					}
					else if (temp.size() == 1) {
						for (int i = 0; i < clauses.size(); i++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(clauses[i]);
							newParent.addChildren(temp[0]);
							clauses[i] = newParent;
						}
						temp.clear();
					}
					else if (temp.size() == 2) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(temp[0]);
						newParent.addChildren(temp[1]);
						clauses.push_back(newParent);
						temp.clear();
					}
				}
				else if (clausesNodes[i].getOperators() == OR) {
					if (temp.size() == 1) {
						ClauseNode newClause = temp[0];
						temp.clear();
						clauses.push_back(newClause);
					}
					else if (temp.size() == 2) {
						ClauseNode newClause1 = temp[0];
						ClauseNode newClause2 = temp[1];
						temp.clear();
						clauses.push_back(newClause1);
						clauses.push_back(newClause2);
					}
				}
			}
			else {
				if (clausesNodes[i].getClauseNodeType() == CLAUSE) {
					temp.push_back(clausesNodes[i]);
				}
			}
		}
	}

	clausesNodes = qc.getPattern();
	if (clausesNodes.size() > 0) {
		std::vector<ClauseNode> temp;
		for (int i = 0; i < clausesNodes.size(); i++) {
			if (clausesNodes[i].getClauseNodeType == OPERATOR) {
				if (clausesNodes[i].getOperators == AND) {
					if (temp.size() == 0) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(pattern[0]);
						newParent.addChildren(pattern[1]);
						pattern.clear();
						pattern.push_back(newParent);
					}
					else if (temp.size() == 1) {
						for (int i = 0; i < pattern.size(); i++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(pattern[i]);
							newParent.addChildren(temp[0]);
							pattern[i] = newParent;
						}
						temp.clear();
					}
					else if (temp.size() == 2) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(temp[0]);
						newParent.addChildren(temp[1]);
						pattern.push_back(newParent);
						temp.clear();
					}
				}
				else if (clausesNodes[i].getOperators == OR) {
					if (temp.size() == 1) {
						ClauseNode newClause = temp[0];
						temp.clear();
						pattern.push_back(newClause);
					}
					else if (temp.size() == 2) {
						ClauseNode newClause1 = temp[0];
						ClauseNode newClause2 = temp[1];
						temp.clear();
						pattern.push_back(newClause1);
						pattern.push_back(newClause2);
					}
				}
			}
			else {
				if (clausesNodes[i].getClauseNodeType == PATTERN) {
					temp.push_back(clausesNodes[i]);
				}
			}
		}
	}

	clausesNodes = qc.getWithClauses();
	if (clausesNodes.size() > 0) {
		std::vector<ClauseNode> temp;
		for (int i = 0; i < clausesNodes.size(); i++) {
			if (clausesNodes[i].getClauseNodeType == OPERATOR) {
				if (clausesNodes[i].getOperators == AND) {
					if (temp.size() == 0) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(withClauses[0]);
						newParent.addChildren(withClauses[1]);
						withClauses.clear();
						withClauses.push_back(newParent);
					}
					else if (temp.size() == 1) {
						for (int i = 0; i < withClauses.size(); i++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(withClauses[i]);
							newParent.addChildren(temp[0]);
							withClauses[i] = newParent;
						}
						temp.clear();
					}
					else if (temp.size() == 2) {
						ClauseNode newParent = ClauseNode(AND);
						newParent.addChildren(temp[0]);
						newParent.addChildren(temp[1]);
						withClauses.push_back(newParent);
						temp.clear();
					}
				}
				else if (clausesNodes[i].getOperators == OR) {
					if (temp.size() == 1) {
						ClauseNode newClause = temp[0];
						temp.clear();
						withClauses.push_back(newClause);
					}
					else if (temp.size() == 2) {
						ClauseNode newClause1 = temp[0];
						ClauseNode newClause2 = temp[1];
						temp.clear();
						withClauses.push_back(newClause1);
						withClauses.push_back(newClause2);
					}
				}
			}
			else {
				if (clausesNodes[i].getClauseNodeType == WITH_CLAUSE) {
					temp.push_back(clausesNodes[i]);
				}
			}
		}
	}

	std::vector<QueryObject> queryObjects;
	int total = clauses.size() * withClauses.size() * pattern.size();
	for (int i = 0; i < total; i++) {
		QueryObject qo;
		queryObjects.push_back(qo);
	}
	for (int i = 0; i < total; i++) {
		int c = i % clauses.size();
		queryObjects[i].setClause(parseClauseTree(clauses[c]));
	}
	for (int i = 0; i < total; i++) {
		int c = i % pattern.size();
		queryObjects[i].setPattern(parsePatternTree(pattern[c]));
	}
	for (int i = 0; i < total; i++) {
		int c = i % withClauses.size();
		queryObjects[i].setWithClause(parseWithClauseTree(withClauses[c]));
	}
	return queryObjectArray;
}

std::vector<Clause> QueryQueuer::parseClauseTree(ClauseNode c) {

	std::vector<Clause> clauses;
	if (c.getClauseNodeType() != OPERATOR) {
		clauses.push_back(c.getClause());
	}
	else {
		std::vector<ClauseNode> children = c.getChildren();
		for (int i = 0; i < children.size(); i++) {
			std::vector<Clause> result = QueryQueuer::parseClauseTree(children[i]);
			clauses.insert(clauses.end(), result.begin(), result.end());
		}
	}

	return clauses;
}

std::vector<Clause> QueryQueuer::parseWithClauseTree(ClauseNode c) {

	std::vector<Clause> withClauses;
	if (c.getClauseNodeType() != OPERATOR) {
		withClauses.push_back(c.getWithClause());
	}
	else {
		std::vector<ClauseNode> children = c.getChildren();
		for (int i = 0; i < children.size(); i++) {
			std::vector<Clause> result = QueryQueuer::parseWithClauseTree(children[i]);
			withClauses.insert(withClauses.end(), result.begin(), result.end());
		}
	}

	return withClauses;
}

std::vector<Pattern> QueryQueuer::parsePatternTree(ClauseNode c) {

	std::vector<Pattern> patterns;
	if (c.getClauseNodeType() != OPERATOR) {
		patterns.push_back(c.getPattern());
	}
	else {
		std::vector<ClauseNode> children = c.getChildren();
		for (int i = 0; i < children.size(); i++) {
			std::vector<Pattern> result = QueryQueuer::parsePatternTree(children[i]);
			patterns.insert(patterns.end(), result.begin(), result.end());
		}
	}

	return patterns;
}

list<string> QueryQueuer::evaluateQueries() {

	list<string> output;
	std::vector<QueryObject> q = parseQueryContent();
	for (int i = 0; i < q.size(); i++) {
		list<string> results;

		QueryObject qo;

		if (validQuery) {
			_evaluator.setQueryObject(q[i]);
			results = _evaluator.evaluateQuery();
			output.insert(output.end(), results.begin(), results.end());
		}
		else {
			results = invalidQueryMessage;
			output.insert(output.end(), results.begin(), results.end());
		}
	}

	return output;
}
