#pragma once
#include "./Param.h"
#include <vector>
#include <map>

struct IntermediateTable {
	bool hasResults;
	vector<Param> tableParams;
	vector<vector<int>> resultsTable;
	vector<Param> withTableParams;

	void instantiateTable();
	void setTableParams(vector<Param> params);
	void addTableParams(Param p);
	void setResultsTable(vector<vector<int>> &table);

	int getParamIndex(Param p);
};
