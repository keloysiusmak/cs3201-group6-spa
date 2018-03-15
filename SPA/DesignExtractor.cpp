#pragma once

using namespace std;

#include "DesignExtractor.h";
#include "Constants.h";
#include "Graph.h"
#include <set>;
#include <queue>;

void DesignExtractor::extract(PKB &pkb) {
	extractNext(pkb);
	extractNextStar(pkb);
}

void DesignExtractor::extractNext(PKB &pkb) {
	
	std::vector<std::vector<int>> data = pkb.getAllStatements();
	int statementCount = data[0].size();
	int currStatement = 1;

	std::vector<int> whileStack;

	while (currStatement <= statementCount) {
		int type = pkb.getFromTable(1, currStatement)[3][0];

		//assignment statement
		if (type == 1 || type == 4) {
			if (pkb.checkFollows(currStatement, currStatement + 1)) {
				std::vector<std::vector<int>> insert = { {currStatement + 1} };
				pkb.insertToTable(NEXT_TABLE, currStatement, insert);
				pkb.insertToTable(NEXT_INVERSE_TABLE, (currStatement + 1), { {currStatement} });
			}
			else if (whileStack.size() > 0) {
				int popped = whileStack.back();
				data.pop_back();
				std::vector<std::vector<int>> insert = { { popped } };
				pkb.insertToTable(NEXT_TABLE, currStatement, insert);
				pkb.insertToTable(NEXT_INVERSE_TABLE, popped, { { currStatement } });
			}
		} 
		else if (type == 2) {
			whileStack.push_back(currStatement);
			int childStmtList = pkb.getFromTable(1, currStatement)[0][1];
			int firstStatementInChild = pkb.getFromTable(2, childStmtList)[1][0];
			int popped = whileStack.back();
			whileStack.pop_back();
			std::vector<std::vector<int>> insert = { { popped } };
			pkb.insertToTable(NEXT_TABLE, currStatement, insert);
			pkb.insertToTable(NEXT_INVERSE_TABLE, popped, { { currStatement } });
		}
		else if (type == 3) {
			for (int a = 1; a < 3; a++) {
				int childStmtList = pkb.getFromTable(1, currStatement)[0][a];
				int firstStatementInChild = pkb.getFromTable(2, childStmtList)[1][0];
				int popped = whileStack.back();
				whileStack.pop_back();
				std::vector<std::vector<int>> insert = { { popped } };
				pkb.insertToTable(NEXT_TABLE, currStatement, insert);
				pkb.insertToTable(NEXT_INVERSE_TABLE, popped, { { currStatement } });
			}
		}

		currStatement++;
	}
	

}

void DesignExtractor::extractNextStar(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllStatements();
	int statementCount = data[0].size();
	int currStatement = 1;
	std::set<int> nextStar; 
	std::queue<int> next;

	while (currStatement <= statementCount) {

		/* Regular */
		std::vector<int> data = pkb.getFromTable(NEXT_TABLE, currStatement)[0];
		while (data.size() > 0) {
			next.push(data.back());
			data.pop_back();
		}
		while (next.size() > 0) {
			int nextStatement = next.back();
			next.pop();
			std::vector<int> data = pkb.getFromTable(NEXT_TABLE, nextStatement)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = nextStar.size();
				int newData = data.back();
				data.pop_back();
				nextStar.insert(newData);
				if (nextStar.size() > initialSize) {
					next.push(newData);
				}
			}
		}

		std::vector<int> insert(nextStar.begin(), nextStar.end());

		pkb.insertToTable(NEXT_STAR_TABLE, currStatement, { insert });
		nextStar.clear();

		/* Inverse */
		data = pkb.getFromTable(NEXT_INVERSE_TABLE, currStatement)[0];
		while (data.size() > 0) {
			next.push(data.back());
			data.pop_back();
		}
		while (next.size() > 0) {
			int nextStatement = next.back();
			next.pop();
			std::vector<int> data = pkb.getFromTable(NEXT_INVERSE_TABLE, nextStatement)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = nextStar.size();
				int newData = data.back();
				data.pop_back();
				nextStar.insert(newData);
				if (nextStar.size() > initialSize) {
					next.push(newData);
				}
			}
		}

		std::vector<int> nextInsert(nextStar.begin(), nextStar.end());

		pkb.insertToTable(NEXT_STAR_INVERSE_TABLE, currStatement, { nextInsert });
		nextStar.clear();

		currStatement++;
	}


}

void DesignExtractor::extractCallsInverse(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	int procCount = data[0].size();
	int currProcedure = 1;
	while (currProcedure <= procCount) {
		std::vector<int> data = pkb.getFromTable(CALLS_TABLE, currProcedure)[0];
		for (int i = 0; i < data.size(); i++) {
			pkb.insertToTable(CALLS_INVERSE_TABLE, data[i], { { currProcedure} });
		}
		currProcedure++;
	}

}

void DesignExtractor::extractCallsStar(PKB &pkb) {

	std::vector<std::vector<int>> data = pkb.getAllProcedures();
	int procCount = data[0].size();
	int currProcedure = 1;
	std::set<int> callsStar;
	std::queue<int> calls;

	while (currProcedure <= procCount) {

		/* Regular */
		std::vector<int> data = pkb.getFromTable(CALLS_TABLE, currProcedure)[0];
		while (data.size() > 0) {
			calls.push(data.back());
			data.pop_back();
		}
		while (calls.size() > 0) {
			int nextProcedure = calls.back();
			calls.pop();
			std::vector<int> data = pkb.getFromTable(CALLS_TABLE, nextProcedure)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = callsStar.size();
				int newData = data.back();
				data.pop_back();
				callsStar.insert(newData);
				if (callsStar.size() > initialSize) {
					calls.push(newData);
				}
			}
		}

		std::vector<int> insert(callsStar.begin(), callsStar.end());

		pkb.insertToTable(CALLS_STAR_TABLE, currProcedure, { insert });
		callsStar.clear();

		/* Inverse */
		data = pkb.getFromTable(CALLS_INVERSE_TABLE, currProcedure)[0];
		while (data.size() > 0) {
			calls.push(data.back());
			data.pop_back();
		}
		while (calls.size() > 0) {
			int nextProcedure = calls.back();
			calls.pop();
			std::vector<int> data = pkb.getFromTable(CALLS_INVERSE_TABLE, nextProcedure)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = callsStar.size();
				int newData = data.back();
				data.pop_back();
				callsStar.insert(newData);
				if (callsStar.size() > initialSize) {
					calls.push(newData);
				}
			}
		}

		std::vector<int> nextInsert(callsStar.begin(), callsStar.end());

		pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, currProcedure, { nextInsert });
		callsStar.clear();

		currProcedure++;
	}
}

void DesignExtractor::extractUsesModifies(PKB &pkb) {
	std::vector<std::vector<int>> data = pkb.getAllProcedures();
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