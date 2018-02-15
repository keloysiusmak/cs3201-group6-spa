#pragma once

#include "QueryObject.h"

using namespace std;


QueryObject::QueryObject() {

}

QueryObject::QueryObject(string query) {

}

void QueryObject::insertDeclaration(string designEntity, string synonym) {

}

void QueryObject::insertClause(string relRef, string firstParam, string secondParam) {

}

void QueryObject::insertPattern(string entityType, string leftParam, string rightParam) {

}

void QueryObject::insertSelectStatement(string synonym) {

}

vector<Declaration> QueryObject::getDeclarations() {
	return _declarations;
}

vector<Clause> QueryObject::getClauses() {
	return _clauses;
}

vector<Pattern> QueryObject::getPatterns() {
	return _Patterns;
}

string QueryObject::getSelectStatement() {
	return _selectStatement;
}
