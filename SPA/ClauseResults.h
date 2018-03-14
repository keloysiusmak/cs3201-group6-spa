#pragma once
#include <unordered_map>
#include "Clause.h"
#include "Pattern.h"

class ClauseResults {
 private:
  /* 2d table storing results from pkb */
  vector<vector<int>> results;
  /* Boolean for when both params are concrete */
  bool valid;
  /* Solely used for pattern */
  vector<int> assignmentsEnts;
 public:
  Param entRef;
  Param lhs;
  Param rhs;

  // Instantiation
  void ClauseResults::instantiateClause(Clause clause);
  void ClauseResults::instantiatePattern(Pattern pattern);

  // Setter methods
  void ClauseResults::setResults(vector<vector<int>> &pkbResults);
  void ClauseResults::setValid(bool validity);

  // Getter methods
  bool ClauseResults::hasResults();
  
};

struct IntermediateTable {
  vector<Param> tableColumns;
  vector<vector<int>> resultsTable;
};
