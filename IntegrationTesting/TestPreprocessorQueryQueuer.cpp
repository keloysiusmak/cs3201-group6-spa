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
				string query6 = "assign a; while w; if ifs; Select a such that Follows(w, a) and Follows(a, ifs)";
				string query7 = "assign a; while w; if ifs; Select a such that Follows(w, a) or Follows(a, ifs)";
				string query8 = "assign a; while w; if ifs; Select a such that (Follows(w, 1) or Follows(a, 5)) and (Follows(if, 2) or Follows(2, 3)";

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

				QueryObject expectedQo6;
				expectedQo6.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo6.insertClause(Follows, WHILE, "w", ASSIGN, "a");
				expectedQo6.insertClause(Follows, ASSIGN, "a", IF, "ifs");

				QueryObject expectedQo7a;
				expectedQo7a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo7a.insertClause(Follows, WHILE, "w", ASSIGN, "a");

				QueryObject expectedQo7b;
				expectedQo7b.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo7b.insertClause(Follows, ASSIGN, "a", IF, "ifs");

				QueryObject expectedQo8a;
				expectedQo8a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo8a.insertClause(Follows, WHILE, "w", ASSIGN, "a");
				expectedQo8a.insertClause(Follows, WHILE, "w", ASSIGN, "a");

				QueryContent qc;
				std::vector<QueryContent> vqc;

				preprocessor.preprocessQuery(query1);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result1 = Utils::compareQueryObjectProperties(expectedQo1, q[0]);
					Assert::AreEqual(true, result1);
				}

				preprocessor.preprocessQuery(query2);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result2 = Utils::compareQueryObjectProperties(expectedQo2, q[0]);
					Assert::AreEqual(true, result2);
				}

				preprocessor.preprocessQuery(query3);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result3 = Utils::compareQueryObjectProperties(expectedQo3, q[0]);
					Assert::AreEqual(true, result3);
				}

				preprocessor.preprocessQuery(query4);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result4 = Utils::compareQueryObjectProperties(expectedQo4, q[0]);
					Assert::AreEqual(true, result4);
				}

				preprocessor.preprocessQuery(query5);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result5 = Utils::compareQueryObjectProperties(expectedQo5, q[0]);
					Assert::AreEqual(true, result5);
				}

				preprocessor.preprocessQuery(query6);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result6 = Utils::compareQueryObjectProperties(expectedQo6, q[0]);
					Assert::AreEqual(true, result6);
				}

				preprocessor.preprocessQuery(query7);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result7 = Utils::compareQueryObjectProperties(expectedQo7a, q[0]) && Utils::compareQueryObjectProperties(expectedQo7b, q[1]);
					Assert::AreEqual(true, result7);
				}
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
	};
}