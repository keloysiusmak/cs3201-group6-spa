#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "PKB.h"
//#include "../SPA/Evaluator.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/Parser.h"

class TestWrapper : public AbstractWrapper {
private:

	//================== Main Component =======================
	PKB pkb;
	//Evaluator evaluator;
	//Preprocessor preprocessor;
	Parser parser;
	DesignExtractor de;
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
