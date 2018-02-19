#pragma once

using namespace std;

#include "PKB.h"

bool PKB::insertToTable(int table_id, string key_id, std::vector<int> column_id, std::vector<std::vector<string>> value)
{
	unordered_map<string, std::vector<std::vector<string>>> table = tables[table_id];

	if (column_id.size() != value.size())
		return false;

	std::unordered_map<string, std::vector<std::vector<string>>>::const_iterator got = table.find(key_id);
	/* Row does not exist */
	if (got == table.end()) {
		int tableValuesCount;
		switch (table_id) {
			case 1:
				tableValuesCount = 4;
			case 2:
				tableValuesCount = 2;
			case 3:
				tableValuesCount = 2;
			case 4:
				tableValuesCount = 2;
			case 5:
				tableValuesCount = 2;
			case 6:
				tableValuesCount = 2;
			case 7:
				tableValuesCount = 1;
		}

		/* Return false if the supplied arguments does not equal to the number of columns specified in the PKB documentation*/
		if (column_id.size() != tableValuesCount)
			return false;

		/* Return false if the supplied argument for column_id is not in increasing order */
		for (int i = 0; i < tableValuesCount - 1; i++) {
			if (column_id[i] > column_id[i+1]) {
				return false;
			}
		}

		std::vector<std::vector<string>> tableValues;
		for (int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}
		
		table.insert({ key_id, tableValues });
	}
	/* Row exists */
	else {
		std::vector<std::vector<string>> tableValues = got->second;
		std::vector<string> data;

		for (int i = 0; i < column_id.size(); i++) {
			data = tableValues[column_id[i]];

			/* Insert every element in vector */
			for (int j = 0; j < value[i].size(); j++) {
				data.push_back(value[i][j]);
			}
			tableValues[column_id[i]] = data;
		}
	}

	
}

std::vector<std::vector<string>> PKB::getFromTable(int table_id, string key_id)
{
	std::vector<std::vector<string>> data;

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