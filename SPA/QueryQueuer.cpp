#include "QueryQueuer.h"

using namespace std;

QueryQueuer::QueryQueuer() {
	bool validQuery = true;
	QueryContent qc;
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

void QueryQueuer::setQueryContent(QueryContent setQc) {
	qc = setQc;
}
QueryContent QueryQueuer::getQueryContent() {
	return qc;
}

list<string> QueryQueuer::evaluateQueries() {

	list<string> results;

	QueryObject qo;

	if (validQuery) {
		_evaluator.setQueryObject(qo);
		results = _evaluator.evaluateQuery();
	}
	else {
		results = invalidQueryMessage;
	}

	return results;
}
