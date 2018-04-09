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
QueryContent QueryQueuer::replaceClauseNode(QueryContent qcr, CLAUSE_SELECTOR clauseOrWithClauseOrPattern, int clauseNo, CLAUSE_LEFT_OR_RIGHT leftOrRight, string val) {
	ClauseNode cn;
	if (clauseOrWithClauseOrPattern == REPLACE_CLAUSE) {
		cn = qcr.getClauses()[clauseNo];
	}
	else if (clauseOrWithClauseOrPattern == REPLACE_WITH_CLAUSE) {
		cn = qcr.getWithClauses()[clauseNo];
	}
	else if (clauseOrWithClauseOrPattern == REPLACE_PATTERN) {
		cn = qcr.getPattern()[clauseNo];
	}
	Clause c = cn.getClause();
	Param pl, pr;
	pl = c.getLeftParam();
	pr = c.getRightParam();
	if (leftOrRight == LEFT_PARAM) {
		pl.value = val;
	}
	else {
		pr.value = val;
	}
	c = Clause(c.getRelRef(), pl, pr, c.getIsInverted());
	cn.setClauseNode(c);

	std::vector<ClauseNode> clauseNodes;
	if (clauseOrWithClauseOrPattern == REPLACE_CLAUSE) {
		clauseNodes = qcr.getClauses();
		clauseNodes[clauseNo] = cn;
		qcr.setClause(clauseNodes);
	}
	else if (clauseOrWithClauseOrPattern == REPLACE_WITH_CLAUSE) {
		clauseNodes = qcr.getWithClauses();
		clauseNodes[clauseNo] = cn;
		qcr.setWithClause(clauseNodes);
	}
	else if (clauseOrWithClauseOrPattern == REPLACE_PATTERN) {
		clauseNodes = qcr.getPattern();
		clauseNodes[clauseNo] = cn;
		qcr.setPattern(clauseNodes);
	}
	return qcr;
}

list<string> QueryQueuer::evaluateQueries() {
	
	std::vector<int> sortedInts = QueryQueuer::sortQueryContent();
	std::vector<QueryContent> sortedQc = QueryQueuer::convertSortedToQC(sortedInts);
	unordered_map<int, std::vector<int>> subQueryMapping = QueryQueuer::getSubQueryMapping();
	list<string> output;

	//indexed by qcId, then indexed by mapping params that need to be updated, and the results obtained from subquery
	unordered_map<int, list<string>> dependencyTable;

	//sort the QueryContent, run it in reverse topological sort
	for (int j = 0; j < sortedQc.size(); j++) {

		int dependencies = sortedQc[j].getChildren().size();
		QueryContent thisQc = sortedQc[j];
		int thisQcId = sortedInts[j];

		int loops;
		std::vector<list<string>> resultList;
		std::vector<std::vector<int>> paramList;
		if (dependencies == 0) loops = 1;
		else {
			std::vector<int> children = thisQc.getChildren();

			loops = 1;
			for (int j = 0; j < children.size(); j++) {

				list<string> result = dependencyTable[children[j]];
				resultList.push_back(result);
				std::vector<int> param = subQueryMapping[children[j]];
				paramList.push_back(param);
				loops *= result.size();
			}
		}
		list<string> results;
		results.clear();
		for (int r = 0; r < loops; r++) {
			for (int d = 0; d < dependencies; d++) {
				int p = r;
				for (int dx = 0; dx < d; dx++) {
					p = p / resultList[dx].size();
				}
				p = p % resultList[d].size();
				list<string>::iterator it = resultList[d].begin();
				std::advance(it, p);
				string value = *it;
				thisQc = QueryQueuer::replaceClauseNode(thisQc, (CLAUSE_SELECTOR)paramList[d][1], paramList[d][2], (CLAUSE_LEFT_OR_RIGHT)paramList[d][3], value);
			}

			std::vector<QueryObject> q = parseQueryContent(thisQc);

			//split into query objects and run
			for (int i = 0; i < q.size(); i++) {

				QueryObject qo;

				if (validQuery) {
					_evaluator.setQueryObject(q[i]);
					list<string> tempResult = _evaluator.evaluateQuery();
					std::copy(results.begin(), results.end(), std::back_insert_iterator<std::list<string>> (tempResult));
				}
				else {
					output.clear();
					results = invalidQueryMessage;
					break;
				}
			}
		}

		//fill in results obtained into any parent query object
		dependencyTable.insert({ sortedInts[j], results});
		output.insert(output.end(), results.begin(), results.end());
	}

	output.unique();

	return output;
}
void QueryQueuer::setSubQueryMapping(unordered_map<int, vector<int>> sqm) {
	subQueryMapping = sqm;
}
unordered_map<int, vector<int>> QueryQueuer::getSubQueryMapping() {
	return subQueryMapping;
}
std::vector<int> QueryQueuer::sortQueryContent() {
	Graph g(qc.size());

	for (int i = 0; i < qc.size(); i++) {
		std::vector<int> children = qc[i].getChildren();

		for (int j = 0; j < children.size(); j++) {
			g.addEdge(i, children[j]);
		}
	}

	std::vector<int> sorted = g.topologicalSort();
	std::reverse(sorted.begin(), sorted.end());
	return sorted;
}
std::vector<QueryContent> QueryQueuer::convertSortedToQC(std::vector<int> sorted) {
	std::vector<QueryContent> output;
	for (int i = 0; i < sorted.size(); i++) {
		output.push_back(qc[sorted[i]]);
	}
	return output;
}
