#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "../SPA/PKB.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/QueryQueuer.h"
#include "../SPA/Parser.h"

class TestWrapper : public AbstractWrapper {
private:

	//================== Main Component =======================
	PKB pkb;
	QueryEvaluator evaluator;
	Preprocessor preprocessor;
	Parser parser;
	DesignExtractor de;
	QueryQueuer queryQueuer;
	//=========================================================

 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();

  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
