#include "./IntermediateTable.h"
using namespace std;

void IntermediateTable::instantiateTable() {
	hasResults = true;
};

void IntermediateTable::setTableParams(vector<Param> params) {
	tableParams = params;
};

void IntermediateTable::addTableParams(Param p) {
	tableParams.push_back(p);
};

void IntermediateTable::setResultsTable(vector<vector<int>> &table) {
	resultsTable = table;
};

//int IntermediateTable::getParamIndex(Param p) {
//	if (tableParams.find(p) != tableParams.end()) return tableParams[p];
//	else return -1;
//};