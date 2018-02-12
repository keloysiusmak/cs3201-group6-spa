#pragma once

using namespace std;

#include "PKB.h"

bool PKB::insertToTable(int table_id, string key_id, std::vector<string> column_id, std::vector<std::vector<string>> value)
{
	unordered_map<string, unordered_map<string, std::vector<string>>> table = tables[table_id];

	if (column_id.size() != value.size())
		return false;

	unordered_map<string, std::vector<string>> data;

	for (int i = 0; i < column_id.size(); i++) {
		data.insert({ column_id[i], value[i] });
	}
}

unordered_map<string, std::vector<string>> getFromTable(int table_id, string key_id)
{
	unordered_map<string, std::vector<string>> data;

	return data;
}

int main()
{
	PKB pkb;
	std::vector<string> columns;
	std::vector<string> values;
	std::vector<std::vector<string>> values_columns;
	columns.push_back("column1");
	values.push_back("v");
	values_columns.push_back(values);
	pkb.insertToTable(1, "0", columns, values_columns);
};