#pragma once

#include "Declaration.h"

using namespace std;

Declaration::Declaration() {

}

void Declaration::setDeclaration(string designEntity, string synonym) {
	_designEntity = designEntity;
	_synonym = synonym;
}

string Declaration::getDesignEntity() {
	return _designEntity;
}

string Declaration::getSynonym() {
	return _synonym;
}