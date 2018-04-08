#pragma once
#include "./Param.h"
#include <vector>
#include <map>

struct IntermediateTable {
	bool hasResults;
	map<Param, int> tableParams;
	vector<vector<int>> resultsTable;

	void instantiateTable();
	void setTableParams(vector<Param> params);
	void setTableParams(map<Param, int> params);
	void addTableParams(Param p);
	void setResultsTable(vector<vector<int>> &table);
	bool tableHasResults();

	int getParamIndex(Param p);
};
