#include "ClauseResults.h"
using namespace std;

void ClauseResults::instantiateClause(Clause clause) {
  lhs = clause.getFirstParam();
  rhs = clause.getSecondParam();
};

void ClauseResults::instantiatePattern(Pattern pattern) {
  entRef = pattern.getEntity();
  lhs = pattern.getLeftParam();
  rhs = pattern.getRightParam();
};

void ClauseResults::setResults(vector<vector<int> > &pkbResults) {
  results = pkbResults;
};

void ClauseResults::setValid(bool validity) {
  valid = validity;
};

bool ClauseResults::hasResults() {
  if (results.size() > 0) return true;
  if (valid) return true;
  return false;
}
