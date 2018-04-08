#include "QueryQueuer.h"

using namespace std;

QueryQueuer::QueryQueuer() {
	bool validQuery = true;
	std::vector<QueryContent> qc;
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

void QueryQueuer::setQueryContent(std::vector<QueryContent> setQc) {
	validQuery = true;
	qc = setQc;
}
std::vector<QueryContent> QueryQueuer::getQueryContent() {
	return qc;
}

void QueryQueuer::setSubQueryMapping(unordered_map<int, Param&> mapping) {
	subQueryMapping = mapping;
}
std::vector<QueryContent> QueryQueuer::getSubQueryMapping() {
	return subQueryMapping;
}

std::vector<QueryObject> QueryQueuer::parseQueryContent(QueryContent qc) {

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
						int sizeC = clauses.size();
						std::vector<ClauseNode> hold1;
						std::vector<ClauseNode> hold2;
						for (int j = 0; j < sizeC / 2; j++) {
							hold1.push_back(clauses[j]);
						}
						for (int j = sizeC / 2; j < sizeC; j++) {
							hold2.push_back(clauses[j]);
						}
						clauses.clear();
						for (int x = 0; x < hold1.size(); x++) {
							for (int y = 0; y < hold2.size(); y++) {
								ClauseNode newParent = ClauseNode(AND);
								newParent.addChildren(hold1[x]);
								newParent.addChildren(hold2[y]);
								clauses.push_back(newParent);
							}
						}
					}
					else if (temp.size() == 1) {
						for (int j = 0; j < clauses.size(); j++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(clauses[j]);
							newParent.addChildren(temp[0]);
							clauses[j] = newParent;
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
		if (temp.size() > 0) {
			ClauseNode newClause1 = temp[0];
			clauses.push_back(newClause1);
		}
	}

	clausesNodes = qc.getPattern();
	if (clausesNodes.size() > 0) {
		std::vector<ClauseNode> temp;
		for (int i = 0; i < clausesNodes.size(); i++) {
			if (clausesNodes[i].getClauseNodeType() == OPERATOR) {
				if (clausesNodes[i].getOperators() == AND) {
					if (temp.size() == 0) {
						int sizeC = pattern.size();
						std::vector<ClauseNode> hold1;
						std::vector<ClauseNode> hold2;
						for (int j = 0; j < sizeC / 2; j++) {
							hold1.push_back(clauses[j]);
						}
						for (int j = sizeC / 2; j < sizeC; j++) {
							hold2.push_back(clauses[j]);
						}
						pattern.clear();
						for (int x = 0; x < hold1.size(); x++) {
							for (int y = 0; y < hold2.size(); y++) {
								ClauseNode newParent = ClauseNode(AND);
								newParent.addChildren(hold1[x]);
								newParent.addChildren(hold2[y]);
								pattern.push_back(newParent);
							}
						}
					}
					else if (temp.size() == 1) {
						for (int j = 0; j < pattern.size(); j++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(pattern[j]);
							newParent.addChildren(temp[0]);
							pattern[j] = newParent;
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
				else if (clausesNodes[i].getOperators() == OR) {
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
				if (clausesNodes[i].getClauseNodeType() == PATTERN) {
					temp.push_back(clausesNodes[i]);
				}
			}
		}
		if (temp.size() > 0) {
			ClauseNode newClause1 = temp[0];
			pattern.push_back(newClause1);
		}
	}

	clausesNodes = qc.getWithClauses();
	if (clausesNodes.size() > 0) {
		std::vector<ClauseNode> temp;
		for (int i = 0; i < clausesNodes.size(); i++) {
			if (clausesNodes[i].getClauseNodeType() == OPERATOR) {
				if (clausesNodes[i].getOperators() == AND) {
					if (temp.size() == 0) {
						int sizeC = withClauses.size();
						std::vector<ClauseNode> hold1;
						std::vector<ClauseNode> hold2;
						for (int j = 0; j < sizeC / 2; j++) {
							hold1.push_back(withClauses[j]);
						}
						for (int j = sizeC / 2; j < sizeC; j++) {
							hold2.push_back(withClauses[j]);
						}
						withClauses.clear();
						for (int x = 0; x < hold1.size(); x++) {
							for (int y = 0; y < hold2.size(); y++) {
								ClauseNode newParent = ClauseNode(AND);
								newParent.addChildren(hold1[x]);
								newParent.addChildren(hold2[y]);
								clauses.push_back(newParent);
							}
						}
					}
					else if (temp.size() == 1) {
						for (int j = 0; j < withClauses.size(); j++) {
							ClauseNode newParent = ClauseNode(AND);
							newParent.addChildren(withClauses[j]);
							newParent.addChildren(temp[0]);
							withClauses[j] = newParent;
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
				else if (clausesNodes[i].getOperators() == OR) {
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
				if (clausesNodes[i].getClauseNodeType() == WITH_CLAUSE) {
					temp.push_back(clausesNodes[i]);
				}
			}
		}
		if (temp.size() > 0) {
			ClauseNode newClause1 = temp[0];
			withClauses.push_back(newClause1);
		}
	}

	std::vector<QueryObject> queryObjects;
	int cSize = (clauses.size() > 0) ? clauses.size() : 1;
	int wSize = (withClauses.size() > 0) ? withClauses.size() : 1;
	int pSize = (pattern.size() > 0) ? pattern.size() : 1;
	int total =  cSize * wSize * pSize;
	if (!(clauses.size() == 0 && withClauses.size() && pattern.size())) {
		for (int i = 0; i < total; i++) {
			QueryObject qo;
			std::vector<Param> p = qc.getSelect();
			for (int x = 0; x < p.size(); x++) {
				qo.insertSelectStmt(p[x].type, p[x].value, p[x].attribute);
			}
			queryObjects.push_back(qo);
		}
		if (clauses.size() > 0) {
			for (int i = 0; i < total; i++) {
				int c = i % cSize;
				queryObjects[i].setClause(parseClauseTree(clauses[c]));
			}
		}
		if (pattern.size() > 0) {
			for (int i = 0; i < total; i++) {
				int c = i % pSize;
				queryObjects[i].setPattern(parsePatternTree(pattern[c]));
			}
		}
		if (withClauses.size() > 0) {
			for (int i = 0; i < total; i++) {
				int c = i % wSize;
				queryObjects[i].setWithClause(parseWithClauseTree(withClauses[c]));
			}
		}
	}
	return queryObjects;
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
	for (int j = 0; j < qc.size(); j++) {
		std::vector<QueryObject> q = parseQueryContent(qc[j]);
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
	}

	return output;
}
