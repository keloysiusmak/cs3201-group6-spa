#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
	//preprocessor.setEvaluator(evaluator);
}

// method for parsing the SIMPLE source

void TestWrapper::parse(std::string filename) {
	pkb = parser.Parse(filename, pkb);
	de.extract(pkb);
	evaluator.setPKB(pkb);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {

	preprocessor.preprocessQuery(query);
	queryQueuer.setEvaluator(evaluator);

	//Invalid Query
	if (preprocessor.getIsErrorExist()) {
		string errorMessage = preprocessor.getErrorMessage();
		queryQueuer.setInvalidQuery(errorMessage);
		results = queryQueuer.evaluateQueries();
	}
	else {
		results.clear();
		std::vector<QueryContent> vqc;
		vqc = preprocessor.getQueryContent();
		queryQueuer.setQueryContent(vqc);
		queryQueuer.setSubQueryMapping(preprocessor.getSubQueryMapping());
		list<string> result = queryQueuer.evaluateQueries();
		results.insert(results.end(), result.begin(), result.end());
	}
}
