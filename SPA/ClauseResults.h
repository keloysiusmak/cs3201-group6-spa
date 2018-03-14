#pragma once
#include <unordered_map>
#include "Clause.h"
#include "Pattern.h"

class ClauseResults {
 public:
  Param entRef;
  Param lhs;
  Param rhs;
  /* 2d table storing results from pkb */
  vector<vector<int>> results;
  /* Boolean for when both params are concrete */
  bool valid;
  /* Solely used for pattern */
  vector<int> assignmentsEnts;

  // Instantiation
  void ClauseResults::instantiateClause(Clause clause);
  void ClauseResults::instantiatePattern(Pattern pattern);

  // Setter methods
  void ClauseResults::setResults(vector<vector<int>> &pkbResults);
  void ClauseResults::setValid(bool validity);
};

struct IntermediateTable {
  vector<Param> tableColumns;
  vector<vector<int>> resultsTable;
};
