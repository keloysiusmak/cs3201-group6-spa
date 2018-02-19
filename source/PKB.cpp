#pragma once

using namespace std;

#include "PKB.h"

bool PKB::insertToTable(int table_id, std::string key_id, std::vector<std::vector<std::string>> value)
{
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[table_id];

	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);
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
		if (value.size() != tableValuesCount)
			return false;

		std::vector<std::vector<std::string>> tableValues;
		for (int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}
		
		table.insert({ key_id, tableValues });
	}
	/* Row exists */
	else {
		std::vector<std::vector<std::string>> tableValues = got->second;
		std::vector<std::string> data;

		for (int i = 0; i < value.size(); i++) {
			data = tableValues[i];

			/* Insert every element in vector */
			for (int j = 0; j < value[i].size(); j++) {
				data.push_back(value[i][j]);
			}
			tableValues[i] = data;
		}
	}

	
}

std::vector<std::vector<std::string>> PKB::getFromTable(int table_id, std::string key_id)
{
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[table_id];

	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);
	/* Row does not exist */
	if (got == table.end()) {
		std::vector<std::vector<std::string>> data;
		return data;
	}
	else {
		return got->second;
	}
}

int main()
{
	PKB pkb;
};