#include "QueryQueuer.h"

using namespace std;

QueryQueuer::QueryQueuer() {
	bool validQuery = true;
	QueryObject qo;
}

void QueryQueuer::setEvaluator(QueryEvaluator &evaluator) {
	_evaluator = evaluator;
}

bool QueryQueuer::isValidQuery() {
	return validQuery;
}

void QueryQueuer::setInvalidQuery(string message) {
	validQuery = false;
	if (message != "") invalidQueryMessage = { message };
	else invalidQueryMessage = {};
};

void QueryQueuer::setQueryObject(QueryObject setQo) {
	qo = setQo;
}
QueryObject QueryQueuer::getQueryObject() {
	return qo;
}

list<string> QueryQueuer::evaluateQueries() {

	list<string> results;

	if (validQuery) {
		_evaluator.setQueryObject(qo);
		results = _evaluator.evaluateQuery();
	}
	else {
		results = invalidQueryMessage;
	}

	return results;
}
