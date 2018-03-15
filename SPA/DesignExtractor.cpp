#pragma once

using namespace std;

#include "DesignExtractor.h";
#include "Constants.h";
#include "Graph.h"
#include <set>;
#include <queue>;

bool DesignExtractor::extract(PKB &pkb) {
	extractNext(pkb);
	extractNextStar(pkb);
	extractCallsInverse(pkb);
	extractCallsStar(pkb);
	//extractUsesModifies(pkb);
	return true;
}

void DesignExtractor::extractNext(PKB &pkb) {
	
	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	for (int i = 0; i < data.size(); i++) {
		std::vector<int> blank;
		DesignExtractor::processStatementList(pkb, pkb.getFromTable(PROC_INFO_TABLE, data[i][0])[0][0], blank, 0);
	}
}

void DesignExtractor::processStatementList(PKB &pkb, int stmtListId, std::vector<int> prevWhile, int parent) {
	std::vector<std::vector<int>> stmts = pkb.getFromTable(STATEMENT_LIST_TABLE, stmtListId);
	for (int i = 0; i < stmts[1].size(); i++) {
		if (i == 0 && parent != 0) {
			pkb.insertToTable(NEXT_TABLE, parent, { { stmts[1][i] } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, stmts[1][i], { { parent } });
		}
		std::vector<std::vector<int>> thisStmt = pkb.getFromTable(STATEMENT_TABLE, stmts[1][i]);
		int type = thisStmt[3][0];

		if (type == 2 || type == 3) {
			if (type == 2) {
				prevWhile.push_back(stmts[1][i]);
				processStatementList(pkb, thisStmt[0][1], prevWhile, stmts[1][i]);
			}
			else if (type == 3) {
				processStatementList(pkb, thisStmt[0][1], prevWhile, stmts[1][i]);
				processStatementList(pkb, thisStmt[0][2], prevWhile, stmts[1][i]);
			}
		}
		else if (i < stmts[1].size() - 1) {
			if (stmts[1][i] == stmts[1][i+1] - 1) {
				pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { stmts[1][i+1] } });
				pkb.insertToTable(NEXT_INVERSE_TABLE, stmts[1][i+1], { { stmts[1][i] } });
			}
		}
		else {
			int bubbleUp = stmts[1][i];
			while (prevWhile.size() > 0) {
				pkb.insertToTable(NEXT_TABLE, bubbleUp, { { prevWhile.back() } });
				pkb.insertToTable(NEXT_INVERSE_TABLE, prevWhile.back(), { { bubbleUp } });
				bubbleUp = prevWhile.back();
				prevWhile.pop_back();
			}
		}
	}
}

void DesignExtractor::extractNextStar(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllStatements();
	if (data.size() > 0) {
		int statementCount = data.size();
		int currStatement = 1;
		std::set<int> nextStar;
		std::queue<int> next;

		while (currStatement <= statementCount) {

			next.empty();

			/* Regular */
			std::vector<std::vector<int>> data = pkb.getFromTable(NEXT_TABLE, currStatement);
			if (data.size() > 0 && data[0].size() > 0) {
				while (data[0].size() > 0) {
					next.push(data[0].back());
					nextStar.insert(data[0].back());
					data[0].pop_back();
				}
				while (next.size() > 0) {
					int nextStatement = next.front();
					next.pop();
					data = pkb.getFromTable(NEXT_TABLE, nextStatement);
					if (data.size() > 0) {
						std::vector<std::vector<int>> tempData = data;
						for (int i = 0; i < tempData[0].size(); i++) {
							int initialSize = nextStar.size();
							int newData = tempData[0][i];
							nextStar.insert(newData);
							if (nextStar.size() > initialSize) {
								next.push(newData);
							}
						}
					}
				}

				std::vector<int> insert(nextStar.begin(), nextStar.end());

				pkb.insertToTable(NEXT_STAR_TABLE, currStatement, { insert });
				nextStar.clear();
			}
			/* Inverse */
			data = pkb.getFromTable(NEXT_INVERSE_TABLE, currStatement);
			nextStar.clear();
			if (data.size() > 0 && data[0].size() > 0) {
				while (data[0].size() > 0) {
					next.push(data[0].back());
					nextStar.insert(data[0].back());
					data[0].pop_back();
				}
				while (next.size() > 0) {
					int nextStatement = next.front();
					next.pop();
					data = pkb.getFromTable(NEXT_INVERSE_TABLE, nextStatement);
					if (data.size() > 0) {
						std::vector<std::vector<int>> tempData = data;
						for (int i = 0; i < tempData[0].size(); i++) {
							int initialSize = nextStar.size();
							int newData = tempData[0][i];
							nextStar.insert(newData);
							if (nextStar.size() > initialSize) {
								next.push(newData);
							}
						}
					}
				}

				std::vector<int> insert(nextStar.begin(), nextStar.end());

				pkb.insertToTable(NEXT_STAR_INVERSE_TABLE, currStatement, { insert });
				nextStar.clear();
			}

			currStatement++;
		}
	}
}

void DesignExtractor::extractCallsInverse(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	if (data.size() > 0) {
		int procCount = data.size();
		int currProcedure = 1;
		while (currProcedure <= procCount) {
			std::vector<std::vector<int>> data = pkb.getFromTable(CALLS_TABLE, currProcedure);
			if (data.size() > 0) {
				for (int i = 0; i < data[0].size(); i++) {
					pkb.insertToTable(CALLS_INVERSE_TABLE, data[0][i], { { currProcedure} });
				}
			}
			currProcedure++;
		}
	}
}

void DesignExtractor::extractCallsStar(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllProcedures();

	if (data.size() > 0) {
		int procCount = data.size();
		int currProcedure = 1;
		std::set<int> callsStar;
		std::queue<int> calls;

		while (currProcedure <= procCount) {

			calls.empty();
			
			/* Regular */
			std::vector<std::vector<int>> data = pkb.getFromTable(CALLS_TABLE, currProcedure);
			if (data.size() > 0 && data[0].size() > 0) {
				while (data[0].size() > 0) {
					calls.push(data[0].back());
					callsStar.insert(data[0].back());
					data[0].pop_back();
				}
				while (calls.size() > 0) {
					int nextProcedure = calls.front();
					calls.pop();
					data = pkb.getFromTable(CALLS_TABLE, nextProcedure);
					if (data.size() > 0 && data[0].size() > 0) {
						std::vector<int> tempData = data[0];
						for (int i = 0; i < tempData.size(); i++) {
							int initialSize = callsStar.size();
							int newData = tempData[i];
							callsStar.insert(newData);
							if (callsStar.size() > initialSize) {
								calls.push(newData);
							}
						}
					}
				}

				std::vector<int> insert(callsStar.begin(), callsStar.end());

				pkb.insertToTable(CALLS_STAR_TABLE, currProcedure, { insert });
				callsStar.clear();
			}
			/* Inverse */
			data = pkb.getFromTable(CALLS_INVERSE_TABLE, currProcedure);
			callsStar.clear();
			if (data.size() > 0 && data[0].size() > 0) {
				while (data[0].size() > 0) {
					calls.push(data[0].back());
					callsStar.insert(data[0].back());
					data[0].pop_back();
				}
				while (calls.size() > 0) {
					int nextProcedure = calls.front();
					calls.pop();
					data = pkb.getFromTable(CALLS_INVERSE_TABLE, nextProcedure);
					if (data.size() > 0 && data[0].size() > 0) {
						std::vector<int> tempData = data[0];
						for (int i = 0; i < tempData.size(); i++) {
							int initialSize = callsStar.size();
							int newData = tempData[i];
							callsStar.insert(newData);
							if (callsStar.size() > initialSize) {
								calls.push(newData);
							}
						}
					}
				}

				std::vector<int> insert(callsStar.begin(), callsStar.end());

				pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, currProcedure, { insert });
				callsStar.clear();
			}

			currProcedure++;
		}
	}
}

void DesignExtractor::extractUsesModifies(PKB &pkb) {
	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	if (data.size() > 0) {
		int procCount = data[0].size();
		Graph g(procCount);
		int currProcedure = 1;

		while (currProcedure <= procCount) {
			std::vector<int> data = pkb.getFromTable(CALLS_TABLE, currProcedure)[0];

			for (int i = 0; i < data.size(); i++) {
				g.addEdge(currProcedure, data[i]);
			}

			currProcedure++;
		}

		std::vector<int> sorted = g.topologicalSort();

		for (int i = 0; i < sorted.size(); i++) {
			std::vector<int> callStmts = pkb.getFromTable(CALLS_TABLE, currProcedure)[1];
			std::vector<int> procUses = pkb.getFromTable(PROC_TABLE, currProcedure)[1];
			std::vector<int> procModifies = pkb.getFromTable(PROC_TABLE, currProcedure)[2];

			std::vector<int> stmts;

			for (int j = 0; j < callStmts.size(); j++) {
				stmts.push_back(callStmts[j]);
				pkb.insertToTable(STATEMENT_TABLE, callStmts[j], { {}, procUses, procModifies, {} });
				std::vector<int> parents = pkb.getParentStar(callStmts[j])[0];
				for (int k = 0; k < parents.size(); k++) {
					stmts.push_back(parents[k]);
					pkb.insertToTable(STATEMENT_TABLE, parents[k], { {}, procUses, procModifies,{} });
				}
				int topStmtListId = pkb.getFromTable(STATEMENT_TABLE, parents[parents.size() - 1])[0][0];
				int proc = pkb.getFromTable(STATEMENT_LIST_TABLE, topStmtListId)[2][0];
				pkb.insertToTable(PROC_INFO_TABLE, proc, { {}, procUses, procModifies });

				for (int m = 0; m < procUses.size(); m++) {
					pkb.insertToTable(USES_TABLE, procUses[m], { stmts, {proc} });
				}
				for (int m = 0; m < procModifies.size(); m++) {
					pkb.insertToTable(USES_TABLE, procModifies[m], { stmts,{ proc } });
				}
			}
		}
	}
}