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

	precomputeWithProcNameVarName(pkb);
	precomputeWithProcNameCallProcName(pkb);
	precomputeWithVarNameCallProcName(pkb);
	precomputeWithStmtNoConstValue(pkb);
	precomputePatternTwoSyn(pkb);

	countFollows(pkb);
	countFollowsStar(pkb);
	countUsesProcedure(pkb);
	countUsesStatement(pkb);
	countModifiesProcedure(pkb);
	countModifiesStatement(pkb);
	countParent(pkb);
	countParentStar(pkb);
	countCalls(pkb);
	countCallsStar(pkb);
	countAffects(pkb);
	countAffectsStar(pkb);
	countNext(pkb);
	countNextStar(pkb);
	countWithProcNameVarName(pkb);
	countWithProcNameCallProcName(pkb);
	countWithVarNameCallProcName(pkb);
	countWithStmtNoConstValue(pkb);
	countPattern(pkb);
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

void DesignExtractor::precomputeWithProcNameVarName(PKB &pkb) {
	std::vector<std::vector<int>> procs = pkb.getAllProcedures();
	for (int i = 0; i < procs.size(); i++) {
		string procName = pkb.getProcedureName(procs[i][0]);
		int varId = pkb.getVariableId(procName);
		if (varId != 0) {
			pkb.insertToTable(PROC_NAME_VAR_NAME_TABLE, procs[i][0], { { varId } });
		}
	}
}

void DesignExtractor::precomputeWithProcNameCallProcName(PKB &pkb) {
	std::vector<std::vector<int>> calls = pkb.getAllStatementsWithType(4);
	for (int i = 0; i < calls.size(); i++) {
		int procId = pkb.getProcedureCalledByCallStatement(calls[i][0])[0][0];
		if (procId != 0) {
			pkb.insertToTable(PROC_NAME_CALL_NAME_TABLE, procId, { { calls[i][0] } });
		}
	}
}

void DesignExtractor::precomputeWithVarNameCallProcName(PKB &pkb) {
	std::vector<std::vector<int>> calls = pkb.getAllStatementsWithType(4);
	for (int i = 0; i < calls.size(); i++) {
		int procId = pkb.getProcedureCalledByCallStatement(calls[i][0])[0][0];
		int varId = pkb.getVariableId(pkb.getProcedureName(procId));
		if (varId != 0) {
			pkb.insertToTable(VAR_NAME_CALL_NAME_TABLE, varId, { { calls[i][0] } });
		}
	}
}

void DesignExtractor::precomputeWithStmtNoConstValue(PKB &pkb) {
	std::vector<std::vector<int>> consts = pkb.getAllConstants();
	int stmtSize = pkb.getAllStatements().size();
	for (int i = 0; i < consts.size(); i++) {
		if (consts[i][0] <= stmtSize) {
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, consts[i][0], { { consts[i][0] } });
		}
	}
}

void DesignExtractor::precomputePatternTwoSyn(PKB &pkb) {

	std::vector<TYPES> types = {ASSIGNMENT_TYPE, WHILE_TYPE, IF_TYPE};

	for (int i = 0; i < types.size(); i++) {
		TYPES t = types[i];
		
		std::vector<std::vector<int>> stmts = pkb.getAllStatementsWithType(t);
		for (int i = 0; i < stmts.size(); i++) {
			if (t == ASSIGNMENT_TYPE) {
				int v;
				v = pkb.getVariableId(pkb.getFromNameTable(PATTERN_TABLE, stmts[i][0])[0]);
				pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, stmts[i][0], { { v } });
			}
			else if (t == WHILE_TYPE) {
				int v;
				v = pkb.getVariableId(pkb.getFromNameTable(PATTERN_TABLE, stmts[i][0])[0]);
				pkb.insertToTable(PATTERN_WHILE_VARIABLE_TABLE, stmts[i][0], { { v } });
			}
			else if (t == IF_TYPE) {
				int v;
				v = pkb.getVariableId(pkb.getFromNameTable(PATTERN_TABLE, stmts[i][0])[0]);
				pkb.insertToTable(PATTERN_IF_VARIABLE_TABLE, stmts[i][0], { { v } });
			}
		}
	}
}

void DesignExtractor::countFollows(PKB &pkb) {
	int size = pkb.getAllFollows().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationFollows, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationFollows, i, 0, pkb.getFollowsAfter(i).size());
		pkb.insertToResultTable(RelationFollows, 0, i, pkb.getFollowsBefore(i).size());
	}
}

void DesignExtractor::countFollowsStar(PKB &pkb) {
	int size = pkb.getAllFollowsStar().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationFollowsStar, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationFollowsStar, i, 0, pkb.getFollowsAfterStar(i).size());
		pkb.insertToResultTable(RelationFollowsStar, 0, i, pkb.getFollowsBeforeStar(i).size());
	}
}

void DesignExtractor::countUsesProcedure(PKB &pkb) {
	int size = pkb.getAllProcedureUsesVariables().size();
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	pkb.insertToResultTable(RelationUsesProcedure, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationUsesProcedure, i, 0, pkb.getUsesVariablesFromProcedure(i).size());
	}

	fields = pkb.getAllVariables();
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationUsesProcedure, 0, i, pkb.getProceduresFromUsesVariable(i).size());
	}
}

void DesignExtractor::countUsesStatement(PKB &pkb) {
	int size = pkb.getAllStatementUsesVariables().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationUsesStatement, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationUsesStatement, i, 0, pkb.getUsesVariablesFromStatement(i).size());
	}

	fields = pkb.getAllVariables();
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationUsesStatement, 0, i, pkb.getStatementsFromUsesVariable(i).size());
	}
}

void DesignExtractor::countModifiesProcedure(PKB &pkb) {
	int size = pkb.getAllProcedureModifiesVariables().size();
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	pkb.insertToResultTable(RelationModifiesProcedure, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationModifiesProcedure, i, 0, pkb.getModifiesVariablesFromProcedure(i).size());
	}

	fields = pkb.getAllVariables();
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationModifiesProcedure, 0, i, pkb.getProceduresFromModifiesVariable(i).size());
	}
}

void DesignExtractor::countModifiesStatement(PKB &pkb) {
	int size = pkb.getAllStatementModifiesVariables().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationModifiesStatement, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationModifiesStatement, i, 0, pkb.getModifiesVariablesFromStatement(i).size());
	}

	fields = pkb.getAllVariables();
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationModifiesStatement, 0, i, pkb.getStatementsFromModifiesVariable(i).size());
	}
}

void DesignExtractor::countParent(PKB &pkb) {
	int size = pkb.getAllParent().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationParent, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationParent, i, 0, pkb.getChildren(i).size());
		std::vector<std::vector<int>> parent = pkb.getParent(i);
		int size = 1;
		if (parent[0][0] == 0) {
			size = 0;
		}
		pkb.insertToResultTable(RelationParent, 0, i, size);
	}
}

void DesignExtractor::countParentStar(PKB &pkb) {
	int size = pkb.getAllParentStar().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationParentStar, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationParentStar, i, 0, pkb.getChildrenStar(i).size());
		pkb.insertToResultTable(RelationParentStar, 0, i, pkb.getParentStar(i).size());
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

void DesignExtractor::countNext(PKB &pkb) {
	int size = pkb.getAllNext().size();
	std::vector<std::vector<int>> fields = pkb.getAllStatements();
	pkb.insertToResultTable(RelationNext, 0, 0, size);
	for (int i = 1; i < fields.size() + 1; i++) {
		pkb.insertToResultTable(RelationNext, i, 0, pkb.getNextAfter(i).size());
		pkb.insertToResultTable(RelationNext, 0, i, pkb.getNextBefore(i).size());
	}
}

void DesignExtractor::countNextStar(PKB &pkb) {
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	int totalRowSize = 0;
	for (int i = 1; i < fields.size() + 1; i++) {
		std::vector<std::vector<int>> rows = pkb.getAllStatementsFromProcedure(i);
		for (int j = 1; j < rows.size() + 1; j++) {
			pkb.insertToResultTable(RelationNextStar, rows[j-1][0], 0, rows.size());
			pkb.insertToResultTable(RelationNextStar, 0, rows[j-1][0], rows.size());
		}
		totalRowSize += rows.size() * rows.size();
	}
	pkb.insertToResultTable(RelationNextStar, 0, 0, totalRowSize);
}

void DesignExtractor::countAffects(PKB &pkb) {
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	int totalRowSize = 0;
	for (int i = 1; i < fields.size() + 1; i++) {
		std::vector<std::vector<int>> rows = pkb.getAllStatementsFromProcedure(i);
		int assignCount = 0;
		for (int j = 1; j < rows.size() + 1; j++) {
			if (pkb.checkStatementHasType(rows[j-1][0], 1)) {
				assignCount++;
			}
		}
		for (int j = 1; j < rows.size() + 1; j++) {
			pkb.insertToResultTable(RelationAffects, rows[j - 1][0], 0, assignCount);
			pkb.insertToResultTable(RelationAffects, 0, rows[j - 1][0], assignCount);
		}
		totalRowSize += assignCount * assignCount;
	}
	pkb.insertToResultTable(RelationAffects, 0, 0, totalRowSize);
}

void DesignExtractor::countAffectsStar(PKB &pkb) {
	std::vector<std::vector<int>> fields = pkb.getAllProcedures();
	int totalRowSize = 0;
	for (int i = 1; i < fields.size() + 1; i++) {
		std::vector<std::vector<int>> rows = pkb.getAllStatementsFromProcedure(i);
		int assignCount = 0;
		for (int j = 1; j < rows.size() + 1; j++) {
			if (pkb.checkStatementHasType(rows[j - 1][0], 1)) {
				assignCount++;
			}
		}
		for (int j = 1; j < rows.size() + 1; j++) {
			pkb.insertToResultTable(RelationAffectsStar, rows[j - 1][0], 0, assignCount);
			pkb.insertToResultTable(RelationAffectsStar, 0, rows[j - 1][0], assignCount);
		}
		totalRowSize += assignCount * assignCount;
	}
	pkb.insertToResultTable(RelationAffectsStar, 0, 0, totalRowSize);
}

void DesignExtractor::countWithProcNameVarName(PKB &pkb) {
	pkb.insertToResultTable(RelationWithName, PROCEDURE, VARIABLE, pkb.getWithProcNameVarName().size());
}

void DesignExtractor::countWithProcNameCallProcName(PKB &pkb) {
	pkb.insertToResultTable(RelationWithName, PROCEDURE, CALL, pkb.getWithProcNameCallProcName().size());
}

void DesignExtractor::countWithVarNameCallProcName(PKB &pkb) {
	pkb.insertToResultTable(RelationWithName, VARIABLE, CALL, pkb.getWithVarNameCallProcName().size());
}

void DesignExtractor::countWithStmtNoConstValue(PKB &pkb) {
	pkb.insertToResultTable(RelationWithConst, 0, 0, pkb.getWithStmtNoConstValue(0).size());
	pkb.insertToResultTable(RelationWithConst, ASSIGNMENT_TYPE, 0, pkb.getWithStmtNoConstValue(ASSIGNMENT_TYPE).size());
	pkb.insertToResultTable(RelationWithConst, WHILE_TYPE, 0, pkb.getWithStmtNoConstValue(WHILE_TYPE).size());
	pkb.insertToResultTable(RelationWithConst, IF_TYPE, 0, pkb.getWithStmtNoConstValue(IF_TYPE).size());
	pkb.insertToResultTable(RelationWithConst, CALL_TYPE, 0, pkb.getWithStmtNoConstValue(CALL_TYPE).size());
}

void DesignExtractor::countPattern(PKB &pkb) {
	pkb.insertToResultTable(RelationPattern, ASSIGNMENT_TYPE, 0, pkb.getAllStatementsWithType(ASSIGNMENT_TYPE).size());
	pkb.insertToResultTable(RelationPattern, WHILE_TYPE, 0, pkb.getAllStatementsWithType(WHILE_TYPE).size());
	pkb.insertToResultTable(RelationPattern, IF_TYPE, 0, pkb.getAllStatementsWithType(IF_TYPE).size());
	pkb.insertToResultTable(RelationPattern, ASSIGNMENT_TYPE, VARIABLE, pkb.getAllStatementsWithType(ASSIGNMENT_TYPE).size());
	pkb.insertToResultTable(RelationPattern, WHILE_TYPE, VARIABLE, pkb.getAllStatementsWithType(WHILE_TYPE).size());
	pkb.insertToResultTable(RelationPattern, IF_TYPE, VARIABLE, pkb.getAllStatementsWithType(IF_TYPE).size());
}