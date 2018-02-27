#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/Evaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PreprocessorEvaluatorIntegrationTesting
{
	Evaluator evaluator;
	Preprocessor preprocessor;

	TEST_CLASS(PreprocessorEvaluatorIntegration) {
		public:

			TEST_CLASS_INITIALIZE(setup) {
				preprocessor.setEvaluator(evaluator);
			}

			//This test method will take in an actual test query
			//and validate & deconstruct the query into a QueryObject.
			//Once the QueryObject is ready, it will be passed to Evaluator
			TEST_METHOD(TestValidPreprocessQuery) {		

				string query1 = "assign a; Select a";
				string query2 = "assign a; Select a pattern a(\"x\", _\"y\"_)";
				string query3 = "assign a; stmt s; Select s such that Follows*(a, 2)";
				string query4 = "stmt s; variable v; Select s such that Modifies(s, v)";
				string query5 = "assign a; while w; Select a such that Follows(w, a) pattern a(\"x\", _)";

				QueryObject expectedQo1;
				expectedQo1.insertSelectStmt(ASSIGN, "a");

				QueryObject expectedQo2;
				expectedQo2.insertSelectStmt(ASSIGN, "a");
				expectedQo2.insertPattern(ASSIGN, "a", IDENT, "x", VAR_NAME, "y");

				QueryObject expectedQo3;
				expectedQo3.insertSelectStmt(STMT, "s");
				expectedQo3.insertClause(FollowsT, ASSIGN, "a", INTEGER, "2");

				QueryObject expectedQo4;
				expectedQo4.insertSelectStmt(STMT, "s");
				expectedQo4.insertClause(ModifiesS, STMT, "s", VARIABLE, "v");

				QueryObject expectedQo5;
				expectedQo5.insertSelectStmt(ASSIGN, "a");
				expectedQo5.insertClause(Follows, WHILE, "w", ASSIGN, "a");
				expectedQo5.insertPattern(ASSIGN, "a", IDENT, "x", ALL, "_");

				preprocessor.preprocessQuery(query1);
				bool result1 = compareQueryObjectProperties(expectedQo1, evaluator.getQueryObject());
				Assert::AreEqual(true, result1);

				preprocessor.preprocessQuery(query2);
				bool result2 = compareQueryObjectProperties(expectedQo2, evaluator.getQueryObject());
				Assert::AreEqual(true, result2);

				preprocessor.preprocessQuery(query3);
				bool result3 = compareQueryObjectProperties(expectedQo3, evaluator.getQueryObject());
				Assert::AreEqual(true, result3);

				preprocessor.preprocessQuery(query4);
				bool result4 = compareQueryObjectProperties(expectedQo4, evaluator.getQueryObject());
				Assert::AreEqual(true, result4);

				preprocessor.preprocessQuery(query5);
				bool result5 = compareQueryObjectProperties(expectedQo5, evaluator.getQueryObject());
				Assert::AreEqual(true, result5);
			}

			//This test method will take in an invalid test query
			//and return invalid message
			TEST_METHOD(TestInvalidPreprocessQuery) {

				string invalidQuery1 = "assign a; Select s"; //synonym not declared
				string invalidQuery2 = "assign a; Select a pattern (_, _)"; //No pattern Type
				string invalidQuery3 = "assign a; stmt s; Select s such that Follows*(a, \"x\")"; //Follows cannot have IDENT on the right param
				string invalidQuery4 = "stmt s; variable v; Select s such that Modifies(v, v)"; //Modifies cannot have variable synonym on the left param
				string invalidQuery5 = "stmt s; variable v; Select s such that Uses(_, v)"; //Uses cannot have _ on the left param

				preprocessor.preprocessQuery(invalidQuery1);
				Assert::AreNotEqual(true, evaluator.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery2);
				Assert::AreNotEqual(true, evaluator.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery3);
				Assert::AreNotEqual(true, evaluator.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery4);
				Assert::AreNotEqual(true, evaluator.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery5);
				Assert::AreNotEqual(true, evaluator.isValidQuery());
			}

			bool compareQueryObjectProperties(QueryObject qo1, QueryObject qo2) {
				bool isSameSelectStatement = (qo1.getSelectStatement().type == qo2.getSelectStatement().type) &&
					(qo1.getSelectStatement().value.compare(qo2.getSelectStatement().value) == 0);

				bool isSameClauses = qo1.getClauses().size() == qo2.getClauses().size();

				if (isSameClauses) {
					for (size_t i = 0; i < qo1.getClauses().size(); i++) {
						Clause c1 = qo1.getClauses().at(i);
						Clause c2 = qo2.getClauses().at(i);

						if (c1.getRelRef() != c2.getRelRef() ||
							c1.getFirstParam().type != c2.getFirstParam().type ||
							c1.getFirstParam().value.compare(c2.getFirstParam().value) != 0 ||
							c1.getSecondParam().type != c2.getSecondParam().type ||
							c1.getSecondParam().value.compare(c2.getSecondParam().value) != 0) {
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
							p1.getRightParam().type != p2.getRightParam().type ||
							p1.getRightParam().value.compare(p2.getRightParam().value) != 0) {
							isSameClauses = false;
							break;
						}
					}
				}

				return isSameSelectStatement && isSameClauses && isSamePatterns;
			}
	};
}