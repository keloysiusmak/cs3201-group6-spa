#pragma once

using namespace std;

#include "DesignExtractor.h";
#include "Constants.h";
#include <set>;
#include <queue>

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
				int popped = whileStack.pop_back;
				std::vector<std::vector<int>> insert = { { popped } };
				pkb.insertToTable(NEXT_TABLE, currStatement, insert);
				pkb.insertToTable(NEXT_INVERSE_TABLE, popped, { { currStatement } });
			}
		} 
		else if (type == 2) {
			whileStack.push_back(currStatement);
			int childStmtList = pkb.getFromTable(1, currStatement)[0][1];
			int firstStatementInChild = pkb.getFromTable(2, childStmtList)[1][0];
			int popped = whileStack.pop_back;
			std::vector<std::vector<int>> insert = { { popped } };
			pkb.insertToTable(NEXT_TABLE, currStatement, insert);
			pkb.insertToTable(NEXT_INVERSE_TABLE, popped, { { currStatement } });
		}
		else if (type == 3) {
			for (int a = 1; a < 3; a++) {
				int childStmtList = pkb.getFromTable(1, currStatement)[0][a];
				int firstStatementInChild = pkb.getFromTable(2, childStmtList)[1][0];
				int popped = whileStack.pop_back;
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
			next.push(data.pop_back);
		}
		while (next.size() > 0) {
			int nextStatement = next.pop;
			std::vector<int> data = pkb.getFromTable(NEXT_TABLE, nextStatement)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = nextStar.size();
				int newData = data.pop_back;
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
		std::vector<int> data = pkb.getFromTable(NEXT_INVERSE_TABLE, currStatement)[0];
		while (data.size() > 0) {
			next.push(data.pop_back);
		}
		while (next.size() > 0) {
			int nextStatement = next.pop;
			std::vector<int> data = pkb.getFromTable(NEXT_INVERSE_TABLE, nextStatement)[0];
			for (int i = 0; i < data.size(); i++) {
				int initialSize = nextStar.size();
				int newData = data.pop_back;
				nextStar.insert(newData);
				if (nextStar.size() > initialSize) {
					next.push(newData);
				}
			}
		}

		std::vector<int> insert(nextStar.begin(), nextStar.end());

		pkb.insertToTable(NEXT_STAR_TABLE, currStatement, { insert });
		nextStar.clear();

		currStatement++;
	}


}
