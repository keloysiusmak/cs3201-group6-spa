#include "./IntermediateTable.h"
using namespace std;

void IntermediateTable::instantiateTable() {
	hasResults = true;
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

int IntermediateTable::getParamIndex(Param p) {
	if (tableParams.find(p) != tableParams.end()) return tableParams[p];
	else return -1;
};