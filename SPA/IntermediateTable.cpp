#include "./IntermediateTable.h"
using namespace std;

void IntermediateTable::instantiateTable() {
	hasResults = false;
};

void IntermediateTable::setTableParams(vector<Param> params) {
	tableParams.clear();
	for (Param p : params) {
		addTableParams(p);
	}
};

void IntermediateTable::setTableParams(map<Param, int> params) {
	tableParams = params;
};

void IntermediateTable::addTableParams(Param p) {
	int index = tableParams.size();
	tableParams[p] = index;
};

void IntermediateTable::setResultsTable(vector<vector<int>> &table) {
	resultsTable = table;
};

bool IntermediateTable::tableHasResults() {
	if (tableParams.size() > 0) {
		return (resultsTable.size() > 0);
	}
	else return hasResults;
}

int IntermediateTable::getParamIndex(Param p) {
	if (tableParams.find(p) != tableParams.end()) return tableParams[p];
	else return -1;
};