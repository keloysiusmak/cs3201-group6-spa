#pragma once

using namespace std;

#include "DesignExtractor.h";
#include "Constants.h";
#include "Graph.h"
#include <set>;
#include <queue>;

bool DesignExtractor::extract(PKB &pkb) {
	extractNext(pkb);
	extractCallsInverse(pkb);
	extractCallStatements(pkb);
	extractCallsStar(pkb);
	extractUsesModifies(pkb);
	/*
	countFollows(pkb);
	countFollowsStar(pkb);
	countUses(pkb);
	countModifies(pkb);
	countParent(pkb);
	countParentStar(pkb);
	*/
	countCalls(pkb);
	countCallsStar(pkb);
	/*
	countAffects(pkb);
	countAffectsStar(pkb);
	countNext(pkb);
	countNextStar(pkb);*/
	return true;
}

void DesignExtractor::extractNext(PKB &pkb) {
	
	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	for (int i = 0; i < data.size(); i++) {
		DesignExtractor::processStatementList(pkb, pkb.getFromTable(PROC_INFO_TABLE, data[i][0])[0][0], 0, 0, 0);
	}
}

void DesignExtractor::processStatementList(PKB &pkb, int stmtListId, int prevWhile, int parent, int nextStmt) {
	std::vector<std::vector<int>> stmts = pkb.getFromTable(STATEMENT_LIST_TABLE, stmtListId);
	for (int i = 0; i < stmts[1].size(); i++) {
		if (i == 0 && parent != 0) {
			pkb.insertToTable(NEXT_TABLE, parent, { { stmts[1][i] } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, stmts[1][i], { { parent } });
		}
		std::vector<std::vector<int>> thisStmt = pkb.getFromTable(STATEMENT_TABLE, stmts[1][i]);
		int type = thisStmt[3][0];

		if (type == 2 || type == 3) {
			int nextStmtPush = 0;
			if (i < stmts[1].size() - 1) {
				nextStmtPush = stmts[1][i + 1];
			}
			if (type == 2) {
				if (nextStmt != 0) {
					pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { nextStmt } });
					pkb.insertToTable(NEXT_INVERSE_TABLE, nextStmt, { { stmts[1][i] } });
				}
				if (nextStmtPush != 0) {
					pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { nextStmtPush } });
					pkb.insertToTable(NEXT_INVERSE_TABLE, nextStmtPush, { { stmts[1][i] } });
				}
				if (nextStmt == 0 && nextStmtPush == 0 && prevWhile != 0) {
					pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { prevWhile } });
					pkb.insertToTable(NEXT_INVERSE_TABLE, prevWhile, { { stmts[1][i] } });
				}
				processStatementList(pkb, thisStmt[0][1], stmts[1][i], stmts[1][i], 0);
			}
			else if (type == 3) {
				if (nextStmtPush != 0) {
					processStatementList(pkb, thisStmt[0][1], prevWhile, stmts[1][i], nextStmtPush);
					processStatementList(pkb, thisStmt[0][2], prevWhile, stmts[1][i], nextStmtPush);
				}
				else {
					if (nextStmt != 0) {
						processStatementList(pkb, thisStmt[0][1], prevWhile, stmts[1][i], nextStmt);
						processStatementList(pkb, thisStmt[0][2], prevWhile, stmts[1][i], nextStmt);
					}
					else {
						processStatementList(pkb, thisStmt[0][1], prevWhile, stmts[1][i], 0);
						processStatementList(pkb, thisStmt[0][2], prevWhile, stmts[1][i], 0);
					}

				}
			}
		}
		else if (i < stmts[1].size() - 1) {
			pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { stmts[1][i+1] } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, stmts[1][i+1], { { stmts[1][i] } });
		}
		else {
			if (nextStmt != 0) {
				pkb.insertToTable(NEXT_TABLE, stmts[1][i], { { nextStmt } });
				pkb.insertToTable(NEXT_INVERSE_TABLE, nextStmt, { { stmts[1][i] } });
			}
			else if (prevWhile != 0) {
				pkb.insertToTable(NEXT_TABLE, stmts[1][i], { {prevWhile} });
				pkb.insertToTable(NEXT_INVERSE_TABLE, prevWhile, { { stmts[1][i] } });
			}
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

void DesignExtractor::extractCallStatements(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	if (data.size() > 0) {
		int procCount = data.size();
		int currProcedure = 1;
		while (currProcedure <= procCount) {
			std::vector<std::vector<int>> data = pkb.getFromTable(CALLS_TABLE, currProcedure);
			if (data.size() > 0) {
				for (int i = 0; i < data[1].size(); i++) {
					pkb.insertToTable(CALL_STATEMENT_TABLE, data[1][i], { { currProcedure } });
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
		int procCount = data.size();
		Graph g(procCount);
		int currProcedure = 1;

		while (currProcedure <= procCount) {
			std::vector<std::vector<int>> data = pkb.getFromTable(CALLS_TABLE, currProcedure);

			if (data.size() > 0) {
				for (int i = 0; i < data[0].size(); i++) {
					g.addEdge(currProcedure - 1, data[0][i] - 1);
				}
			}

			currProcedure++;
		}

		std::vector<int> sorted = g.topologicalSort();
		std::vector<int> newSorted;

		for (int i = sorted.size() - 1; i >= 0; i--) {
			newSorted.push_back(sorted[i] + 1);
		}

		sorted = newSorted;

		for (int i = 0; i < sorted.size(); i++) {
			currProcedure = sorted[i];
			std::vector<std::vector<int>> callStmtsPre = pkb.getFromTable(CALLS_TABLE, currProcedure);
			std::vector<int> callStmts = {};
			callStmts.clear();
			if (callStmtsPre.size() > 1 && callStmtsPre[1].size() > 0) {
				callStmts = callStmtsPre[1];
			}
			std::vector<int> procUses;
			std::vector<int> procModifies;

			if (callStmts.size() > 0) {
				std::vector<std::vector<int>> procUsesPre = pkb.getFromTable(PROC_INFO_TABLE, currProcedure);
				if (procUsesPre.size() > 0) {
					procUses = pkb.getFromTable(PROC_INFO_TABLE, currProcedure)[1];
				}

				std::vector<std::vector<int>> procModifiesPre = pkb.getFromTable(PROC_INFO_TABLE, currProcedure);
				if (procModifiesPre.size() > 0) {
					procModifies = pkb.getFromTable(PROC_INFO_TABLE, currProcedure)[2];
				}
			}

			std::vector<int> stmts;

			for (int j = 0; j < callStmts.size(); j++) {
				stmts.push_back(callStmts[j]);
				pkb.insertToTable(STATEMENT_TABLE, callStmts[j], { {}, procUses, procModifies, {} });
				std::vector<std::vector<int>> parents = pkb.getParentStar(callStmts[j]);
				for (int k = 0; k < parents.size(); k++) {
					stmts.push_back(parents[k][0]);
					pkb.insertToTable(STATEMENT_TABLE, parents[k][0], { {}, procUses, procModifies,{} });
				}
				int topStmtListId = pkb.getFromTable(STATEMENT_TABLE, parents[parents.size() - 1][0])[0][0];
				std::vector<std::vector<int>> proc = pkb.getFromTable(STATEMENT_LIST_TABLE, topStmtListId);
				if (proc.size() > 2 && proc[2].size() > 0) {
					pkb.insertToTable(PROC_INFO_TABLE, proc[2][0], { {}, procUses, procModifies });
				}
				for (int m = 0; m < procUses.size(); m++) {
					pkb.insertToTable(USES_TABLE, procUses[m], { {stmts }, { proc[2][0] }});
				}
				for (int m = 0; m < procModifies.size(); m++) {
					pkb.insertToTable(MODIFIES_TABLE, procModifies[m], { {stmts},{ proc[2][0] } });
				}
			}
		}
	}
}

void DesignExtractor::countCalls(PKB &pkb) {
	int size = pkb.getAllCalls().size();
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	pkb.insertToResultTable(RelationCalls, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationCalls, i, 0, pkb.getCallsAfter(i).size());
		pkb.insertToResultTable(RelationCalls, 0, i, pkb.getCallsBefore(i).size());
	}
}

void DesignExtractor::countCallsStar(PKB &pkb) {
	int size = pkb.getAllCallsStar().size();
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	pkb.insertToResultTable(RelationCallsStar, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationCallsStar, i, 0, pkb.getCallsAfterStar(i).size());
		pkb.insertToResultTable(RelationCallsStar, 0, i, pkb.getCallsBeforeStar(i).size());
	}
}