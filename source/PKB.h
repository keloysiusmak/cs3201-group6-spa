#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::tr1;

class PKB {
public:
	bool insertToTable(int table_id, string key_id, std::vector<string> column_id, std::vector<std::vector<string>> value);
	unordered_map<string, std::vector<string>> getFromTable(int table_id, string key_id);
	bool modifyTable(int table_id, string key_id, std::vector<string> column_id, std::vector<std::vector<string>> value);

	unordered_map<string, unordered_map<string, std::vector<string>>> tables[6];
};