#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::tr1;

class PKB {
public:
	/*  PKB Structure*/
	unordered_map<std::string, std::vector<std::vector<std::string>>> tables[6];

	/* PKB Operations */
	bool insertToTable(int, std::string, std::vector<std::vector<std::string>>);
	std::vector<std::vector<std::string>> getFromTable(int, std::string);
	
	/* Follows Operations */
	int getFollowsBefore(int);
	int getFollowsAfter(int);
	std::vector<int> getFollowsBeforeStar(int);
	std::vector<int> getFollowsAfterStar(int);
	bool checkFollows(int, int);
	bool checkFollowsStar(int, int);
	std::vector<int[2]> getAllFollows();
	std::vector<unordered_map<int, std::vector<int>>> getAllFollowsStar();

};