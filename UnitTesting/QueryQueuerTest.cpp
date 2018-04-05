#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/Pattern.h"
#include "../SPA/QueryQueuer.h";
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{
	TEST_CLASS(QueryQueuerTest)
	{
	public:
		
		TEST_METHOD(QueryQueuerParseQueryContent)
		{
			PKB pkb;
			QueryQueuer qq;

			QueryContent qc;
			qc.insertClause(Modifies, VARIABLE, "v", IDENT, "x");
			qc.insertClause(Uses, VARIABLE, "v", IDENT, "y");
			qc.insertOperator(CLAUSE, OR);

			qq.setQueryContent(qc);
			std::vector<QueryObject> result = qq.parseQueryContent();
			QueryObject expectedQo1;
			expectedQo1.insertClause(Modifies, VARIABLE, "v", IDENT, "x");
			QueryObject expectedQo2;
			expectedQo2.insertClause(Modifies, VARIABLE, "v", IDENT, "y");
			Assert::AreEqual(true, compareQueryObjectProperties(result[0], expectedQo1));
			Assert::AreEqual(true, compareQueryObjectProperties(result[1], expectedQo2));

		}

		bool compareQueryObjectProperties(QueryObject qo1, QueryObject qo2) {

			bool isSameSelectStatement = qo1.getSelectStatements().size() == qo2.getSelectStatements().size();

			if (isSameSelectStatement) {
				for (size_t i = 0; i < qo1.getSelectStatements().size(); i++) {
					Param s1 = qo1.getSelectStatements().at(i);
					Param s2 = qo1.getSelectStatements().at(i);

					if (s1.type != s2.type ||
						s1.value.compare(s2.value) != 0 ||
						s1.attribute != s2.attribute) {
						isSameSelectStatement = false;
					}
				}
			}

			bool isSameClauses = qo1.getClauses().size() == qo2.getClauses().size();

			if (isSameClauses) {
				for (size_t i = 0; i < qo1.getClauses().size(); i++) {
					Clause c1 = qo1.getClauses().at(i);
					Clause c2 = qo2.getClauses().at(i);

					if (c1.getRelRef() != c2.getRelRef() ||
						c1.getLeftParam().type != c2.getLeftParam().type ||
						c1.getLeftParam().value.compare(c2.getLeftParam().value) != 0 ||
						c1.getLeftParam().attribute != c2.getLeftParam().attribute ||
						c1.getRightParam().type != c2.getRightParam().type ||
						c1.getRightParam().value.compare(c2.getRightParam().value) != 0 ||
						c1.getRightParam().attribute != c2.getRightParam().attribute) {
						isSameClauses = false;
						break;
					}
				}
			}

			bool isSamePatterns = qo1.getPatterns().size() == qo2.getPatterns().size();

			if (isSamePatterns) {
				for (size_t i = 0; i < qo1.getPatterns().size(); i++) {
					Pattern p1 = qo1.getPatterns().at(i);
					Pattern p2 = qo2.getPatterns().at(i);

					if (p1.getEntity().type != p2.getEntity().type ||
						p1.getEntity().value.compare(p2.getEntity().value) != 0 ||
						p1.getLeftParam().type != p2.getLeftParam().type ||
						p1.getLeftParam().value.compare(p2.getLeftParam().value) != 0 ||
						p1.getLeftParam().attribute != p2.getLeftParam().attribute ||
						p1.getRightParam().type != p2.getRightParam().type ||
						p1.getRightParam().value.compare(p2.getRightParam().value) != 0 ||
						p1.getRightParam().attribute != p2.getRightParam().attribute) {
						isSameClauses = false;
						break;
					}
				}
			}

			bool isSameWithClauses = qo1.getWithClauses().size() == qo2.getWithClauses().size();

			if (isSameWithClauses) {
				for (size_t i = 0; i < qo1.getWithClauses().size(); i++) {
					Clause w1 = qo1.getWithClauses().at(i);
					Clause w2 = qo2.getWithClauses().at(i);

					if (w1.getRelRef() != w2.getRelRef() ||
						w1.getLeftParam().type != w2.getLeftParam().type ||
						w1.getLeftParam().value.compare(w2.getLeftParam().value) != 0 ||
						w1.getLeftParam().attribute != w2.getLeftParam().attribute ||
						w1.getRightParam().type != w2.getRightParam().type ||
						w1.getRightParam().value.compare(w2.getRightParam().value) != 0 ||
						w1.getRightParam().attribute != w2.getRightParam().attribute) {
						isSameWithClauses = false;
						break;
					}
				}
			}

			return isSameSelectStatement && isSameClauses && isSamePatterns && isSameWithClauses;
		}


	};

}