#include "Parent.h"

using namespace std;

Parent::Parent()
{

}

//get Parent table result from PKB
array<int, list<int>> Parent::getParentTable() {
	return tableOne; //or parentTable?
}

bool Parent::isFirstParamValid(int firstParam, int secondParam) {
	//check if parent exists in tableOne/parentTable
	//return iterator to the end if parent doesn't exist in tableOne
	if (tableOne.get(firstParam) == tableOne.end()) {
		return false;
	}
	else {
		return true;
	}
}

//if there is a reverse parent table
bool Parent::isSecondParamValid(int firstParam, int secondParam) {
	if (tableOne.get(SecondParam) == tableOneReverse.end()) {
		return false;
	}
	else {
		return true;
	}
}

//if both params are given
bool Parent::isParent(int firstParam, list<int> secondParam) {
	list<int> parentOfVector = tableOne.at(firstParam);
	//check if secondParam is in the parentOf list of firstParam
	if ((parentOfVector.begin(), parentOfVector.end(), secondParam) != parentOfVector.end()) {
		return true;
	}
	else {
		return false;
	}
}

//if only first param is given
list<int> Parent::getChildren(int firstParam) {
	list<int> parentOfList = tableOne.at(firstParam);
	return list<int>;
}

//if only second param is given
int Parent::getParent(int secondParam) {
	int parentOf = tableOneReverse.at(secondParam);
	return parentOf;
}

//if none of the params are given
list<int, int> Parent::getAllParentPairs() {
	//tableOne.begin, tableOne.end
	return list<int, int>;
}
