#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::tr1;

class PKB {
public:
	/*PKB Structure*/
	unordered_map<string, std::vector<std::vector<string>>> tables[6];

	/*PKB Operations*/
	bool insertToTable(int table_id, string key_id, std::vector<int> column_id, std::vector<std::vector<string>> value);
	std::vector<std::vector<string>> getFromTable(int table_id, string key_id);
	
	/**/
};