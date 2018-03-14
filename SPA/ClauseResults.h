#pragma once
#include <unordered_map>
#include "Clause.h"
#include "Pattern.h"

class ClauseResults {
 private:
  /* Boolean for when both params are concrete */
  bool valid;
  /* Solely used for pattern */
  vector<int> assignmentsEnts;
 public:
  Param entRef;
  Param lhs;
  Param rhs;

  /* 2d table storing results from pkb */
  vector<Param> tableParams;
  vector<vector<int>> results;

  // Instantiation
  void ClauseResults::instantiateClause(Clause clause);
  void ClauseResults::instantiatePattern(Pattern pattern);

  // Setter methods
  void ClauseResults::setResults(vector<vector<int>> &pkbResults);
  void ClauseResults::setValid(bool validity);

  // Getter methods
  bool ClauseResults::isValid();
  bool ClauseResults::hasResults();
  int ClauseResults::numSyns();
  
};

struct IntermediateTable {
  vector<Param> tableParams;
  vector<vector<int>> resultsTable;

  void addTableParams(Param p) {
	tableParams.push_back(p);
  };

  void setResultsTable(vector<vector<int>> &table) {
	resultsTable = table;
  }
};
