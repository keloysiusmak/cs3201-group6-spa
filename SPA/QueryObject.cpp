#pragma once

#include "QueryObject.h"

using namespace std;


QueryObject::QueryObject() {

}

void QueryObject::insertDeclaration(string designEntity, string synonym) {

}

void QueryObject::insertSuchThat(string relRef, string firstParam, string secondParam) {

}

void QueryObject::insertPattern(string entityType, string leftParam, string rightParam) {

}

void QueryObject::insertSelectStatement(string synonym) {

}

vector<Declaration> QueryObject::getDeclarations() {
	return _declarations;
}

vector<SuchThat> QueryObject::getSuchThats() {
	return _suchThats;
}

vector<Pattern> QueryObject::getPatterns() {
	return _Patterns;
}

string QueryObject::getSelectStatement() {
	return _selectStatement;
}
