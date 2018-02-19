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
	std::string getFollowsBefore(std::string);
	std::string getFollowsAfter(std::string);
	std::vector<std::string> getFollowsBeforeStar(std::string);
	std::vector<std::string> getFollowsAfterStar(std::string);
	bool checkFollows(std::string, std::string);
	bool checkFollowsStar(std::string, std::string);
	std::vector<std::string[2]> getAllFollows();
	unordered_map<std::string, std::vector<std::string>> getAllFollowsStar();

	/* Parent Operations */
	std::string getParent(std::string);
	std::vector<std::string> getParentStar(std::string);
	std::vector<std::string> getChildren(std::string);
	std::vector<std::string> getChildrenStar(std::string);
	bool checkParent(std::string, std::string);
	bool checkParentStar(std::string, std::string);
	std::vector<std::string[2]> getAllParent();
	unordered_map<std::string, std::vector<std::string>> getAllParentStar();

};