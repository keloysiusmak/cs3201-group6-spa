#pragma once

#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

class Parent
{
public:
	Parent();
	map<int, list<int>> getParentTable();
	bool isFirstParamValid(int, int);
	bool isSecondParamValid(int, int);
	bool isParent(int, list<int>);
	list<int> getChildren(int);
	list<int> getParent(int);
	list<int, int> getAllParentPairs();

private:
	map<int, list<int>> tableOne;
	map<int, list<int>> tableOneReverse;

};