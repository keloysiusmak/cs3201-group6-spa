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

void IntermediateTable::replaceTableParam(Param original, Param replacement) {
	int paramIndex = getParamIndex(original);
	Param* originalParamPointer = &(getParamFromIndex(paramIndex));
	originalParamPointer->type = replacement.type;
	originalParamPointer->value = replacement.value;
	originalParamPointer->attribute = replacement.attribute;
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

AttrType IntermediateTable::getParamAttr(Param p) {
	for (pair<Param, int> tableParamIndex : tableParams) {
		Param tableParam = tableParamIndex.first;
		if (tableParam.type == p.type && tableParam.value == p.value) {
			return tableParam.attribute;
		}
	}
};

int IntermediateTable::getParamIndex(Param p) {
	if (tableParams.find(p) != tableParams.end()) return tableParams[p];
	else return -1;
};

Param IntermediateTable::getParamFromIndex(int index) {
	for (pair<Param, int> paramIndex : tableParams) {
		if (paramIndex.second == index) return paramIndex.first;
	}
};