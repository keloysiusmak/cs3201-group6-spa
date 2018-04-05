#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/QueryQueuer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PreprocessorEvaluatorIntegrationTesting
{
	QueryQueuer queryQueuer;
	Preprocessor preprocessor;

	TEST_CLASS(PreprocessorEvaluatorIntegration) {
		public:

			TEST_CLASS_INITIALIZE(setup) {
			}

			//This test method will take in an actual test query
			//and validate & deconstruct the query into a QueryObject.
			//Once the QueryObject is ready, it will be passed to Evaluator
			TEST_METHOD(PreprocessorEvaluatorValidPreprocessQuery) {		

				string query1 = "assign a; Select a";
				string query2 = "assign a; Select a pattern a(\"x\", _\"y + 1\"_)";
				string query3 = "assign a; stmt s; Select s such that Follows*(a, 2)";
				string query4 = "stmt s; variable v; Select s such that Modifies(s, v)";
				string query5 = "assign a; while w; Select a such that Follows(w, a) pattern a(\"x\", _)";

				QueryObject expectedQo1;
				expectedQo1.insertSelectStmt(ASSIGN, "a", NONE);

				QueryObject expectedQo2;
				expectedQo2.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo2.insertPattern(ASSIGN, "a", VAR_IDENT, "x", EXPR, "y|1|+|");

				QueryObject expectedQo3;
				expectedQo3.insertSelectStmt(STMT, "s", NONE);
				expectedQo3.insertClause(FollowsT, ASSIGN, "a", INTEGER, "2");

				QueryObject expectedQo4;
				expectedQo4.insertSelectStmt(STMT, "s", NONE);
				expectedQo4.insertClause(Modifies, STMT, "s", VARIABLE, "v");

				QueryObject expectedQo5;
				expectedQo5.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo5.insertClause(Follows, WHILE, "w", ASSIGN, "a");
				expectedQo5.insertPattern(ASSIGN, "a", VAR_IDENT, "x", ALL, "_");

				QueryObject qo;

				preprocessor.preprocessQuery(query1);
				qo = preprocessor.getQueryObject();
				queryQueuer.setQueryObject(qo);
				bool result1 = compareQueryObjectProperties(expectedQo1, queryQueuer.getQueryObject());
				Assert::AreEqual(true, result1);

				preprocessor.preprocessQuery(query2);
				qo = preprocessor.getQueryObject();
				queryQueuer.setQueryObject(qo);
				bool result2 = compareQueryObjectProperties(expectedQo2, queryQueuer.getQueryObject());
				Assert::AreEqual(true, result2);

				preprocessor.preprocessQuery(query3);
				qo = preprocessor.getQueryObject();
				queryQueuer.setQueryObject(qo);
				bool result3 = compareQueryObjectProperties(expectedQo3, queryQueuer.getQueryObject());
				Assert::AreEqual(true, result3);

				preprocessor.preprocessQuery(query4);
				qo = preprocessor.getQueryObject();
				queryQueuer.setQueryObject(qo);
				bool result4 = compareQueryObjectProperties(expectedQo4, queryQueuer.getQueryObject());
				Assert::AreEqual(true, result4);

				preprocessor.preprocessQuery(query5);
				qo = preprocessor.getQueryObject();
				queryQueuer.setQueryObject(qo);
				bool result5 = compareQueryObjectProperties(expectedQo5, queryQueuer.getQueryObject());
				Assert::AreEqual(true, result5);
			}

			//This test method will take in an invalid test query
			//and return invalid message
			TEST_METHOD(PreprocessorEvaluatorInvalidPreprocessQuery) {

				string invalidQuery1 = "assign a; Select s"; //synonym not declared
				string invalidQuery2 = "assign a; Select a pattern (_, _)"; //No pattern Type
				string invalidQuery3 = "assign a; stmt s; Select s such that Follows*(a, \"x\")"; //Follows cannot have IDENT on the right param
				string invalidQuery4 = "stmt s; variable v; Select s such that Modifies(v, v)"; //Modifies cannot have variable synonym on the left param
				string invalidQuery5 = "stmt s; variable v; Select s such that Uses(_, v)"; //Uses cannot have _ on the left param
				string invalidQuery6 = "stmt s; variable v; Select s such  that Uses(_, v)"; //Such That can only have single space in between

				preprocessor.preprocessQuery(invalidQuery1);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery2);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery3);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery4);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery5);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());

				preprocessor.preprocessQuery(invalidQuery6);
				queryQueuer.setInvalidQuery(preprocessor.getErrorMessage());
				Assert::AreNotEqual(true, queryQueuer.isValidQuery());
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