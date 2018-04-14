#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/QueryQueuer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PreprocessorQueryQueuerIntegrationTesting
{
	QueryQueuer queryQueuer;
	Preprocessor preprocessor;

	TEST_CLASS(PreprocessorQueryQueuerIntegration) {
		public:

			TEST_CLASS_INITIALIZE(setup) {
			}

			//This test method will take in an actual test query
			//and validate & deconstruct the query into a QueryObject.
			//Once the QueryObject is ready, it will be passed to Evaluator
			TEST_METHOD(PreprocessorQueryQueuerValidPreprocessQuery) {		

				string query1 = "assign a; Select a";
				string query2 = "assign a; Select a pattern a(\"x\", _\"y + 1\"_)";
				string query3 = "assign a; stmt s; Select s such that Follows*(a, 2)";
				string query4 = "stmt s; variable v; Select s such that Modifies(s, v)";
				string query5 = "assign a; while w; Select a such that Follows(w, a) pattern a(\"x\", _)";
				string query6 = "assign a; while w; if ifs; Select a such that Follows(w, a) and Follows(a, ifs)";
				string query7 = "assign a; while w; if ifs; Select a such that Follows(w, a) or Follows(a, ifs)";
				string query8 = "assign a; while w; if ifs; Select a such that (Follows(w, 1) or Follows(a, 5)) and (Follows(ifs, 2) or Follows(2, 3))";
				string query9 = "if ifs; variable v; assign a; while w; Select w pattern(w(\"g\", _) or ifs(v, _, _)) and (a(v, _\"x\"_) or a(v, _\"y\"_))";

				QueryObject expectedQo1;
				expectedQo1.insertSelectStmt(ASSIGN, "a", NONE);

				QueryObject expectedQo2;
				expectedQo2.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo2.insertPattern(ASSIGN, "a", VAR_IDENT, "x", EXPR, "|y|1|+|", false);

				QueryObject expectedQo3;
				expectedQo3.insertSelectStmt(STMT, "s", NONE);
				expectedQo3.insertClause(FollowsT, ASSIGN, "a", INTEGER, "2", false);

				QueryObject expectedQo4;
				expectedQo4.insertSelectStmt(STMT, "s", NONE);
				expectedQo4.insertClause(Modifies, STMT, "s", VARIABLE, "v", false);

				QueryObject expectedQo5;
				expectedQo5.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo5.insertClause(Follows, WHILE, "w", ASSIGN, "a", false);
				expectedQo5.insertPattern(ASSIGN, "a", VAR_IDENT, "x", ALL, "_", false);

				QueryObject expectedQo6;
				expectedQo6.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo6.insertClause(Follows, WHILE, "w", ASSIGN, "a", false);
				expectedQo6.insertClause(Follows, ASSIGN, "a", IF, "ifs", false);

				QueryObject expectedQo7a;
				expectedQo7a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo7a.insertClause(Follows, WHILE, "w", ASSIGN, "a", false);

				QueryObject expectedQo7b;
				expectedQo7b.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo7b.insertClause(Follows, ASSIGN, "a", IF, "ifs", false);

				QueryObject expectedQo8a;
				expectedQo8a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo8a.insertClause(Follows, WHILE, "w", INTEGER, "1", false);
				expectedQo8a.insertClause(Follows, IF, "ifs", INTEGER, "2", false);

				QueryObject expectedQo8b;
				expectedQo8b.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo8b.insertClause(Follows, WHILE, "w", INTEGER, "1", false);
				expectedQo8b.insertClause(Follows, INTEGER, "2", INTEGER, "3", false);

				QueryObject expectedQo8c;
				expectedQo8c.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo8c.insertClause(Follows, ASSIGN, "a", INTEGER, "5", false);
				expectedQo8c.insertClause(Follows, IF, "ifs", INTEGER, "2", false);

				QueryObject expectedQo8d;
				expectedQo8d.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo8d.insertClause(Follows, ASSIGN, "a", INTEGER, "5", false);
				expectedQo8d.insertClause(Follows, INTEGER, "2", INTEGER, "3", false);
				
				QueryObject expectedQo9a;
				expectedQo9a.insertSelectStmt(WHILE, "w", NONE);
				expectedQo9a.insertPattern(WHILE, "w", VAR_IDENT, "g", ALL, "_", false);
				expectedQo9a.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "|x|", false);

				QueryObject expectedQo9b;
				expectedQo9b.insertSelectStmt(WHILE, "w", NONE);
				expectedQo9b.insertPattern(WHILE, "w", VAR_IDENT, "g", ALL, "_", false);
				expectedQo9b.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "|y|", false);

				QueryObject expectedQo9c;
				expectedQo9c.insertSelectStmt(WHILE, "w", NONE);
				expectedQo9c.insertPattern(IF, "ifs", VARIABLE, "v", ALL, "_", false);
				expectedQo9c.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "|x|", false);

				QueryObject expectedQo9d;
				expectedQo9d.insertSelectStmt(WHILE, "w", NONE);
				expectedQo9d.insertPattern(IF, "ifs", VARIABLE, "v", ALL, "_", false);
				expectedQo9d.insertPattern(ASSIGN, "a", VARIABLE, "v", EXPR, "|y|", false);

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

				preprocessor.preprocessQuery(query8);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result8 = Utils::compareQueryObjectProperties(expectedQo8a, q[0]) && Utils::compareQueryObjectProperties(expectedQo8b, q[1]) && Utils::compareQueryObjectProperties(expectedQo8c, q[2]) && Utils::compareQueryObjectProperties(expectedQo8d, q[3]);
					Assert::AreEqual(true, result8);
				}

				preprocessor.preprocessQuery(query9);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result9 = Utils::compareQueryObjectProperties(expectedQo9a, q[0]) && Utils::compareQueryObjectProperties(expectedQo9b, q[1]) && Utils::compareQueryObjectProperties(expectedQo9c, q[2]) && Utils::compareQueryObjectProperties(expectedQo9d, q[3]);
					Assert::AreEqual(true, result9);
				}
			}

			//This test method will take in an invalid test query
			//and return invalid message
			TEST_METHOD(PreprocessorQueryQueuerInvalidPreprocessQuery) {

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

			//This test method will take in an actual test query
			//and validate & deconstruct the query into a QueryObject.
			//Once the QueryObject is ready, it will be passed to Evaluator
			TEST_METHOD(PreprocessorQueryQueuerValidPreprocessQuerySubQuery) {

				std::vector<QueryContent> vqc;

				string query1 = "assign a; if ifs; Select a such that Follows(a,(Select ifs such that Uses(ifs, \"x\")))";
				
				QueryObject expectedQo1a;
				expectedQo1a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo1a.insertClause(Follows, ASSIGN, "a", INTEGER, "1", false);

				QueryObject expectedQo1b;
				expectedQo1b.insertSelectStmt(IF, "ifs", NONE);
				expectedQo1b.insertClause(Uses, IF, "ifs", VAR_IDENT, "x", false);

				std::vector<QueryObject> vqo;
				vqo.push_back(expectedQo1b);
				vqo.push_back(expectedQo1a);

				preprocessor.preprocessQuery(query1);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result1 = Utils::compareQueryObjectProperties(vqo[i], q[0]);
					Assert::AreEqual(true, result1);
				}

				string query2 = "assign a; if ifs; Select a such that Follows((Select a such that Modifies(a, \"y\")),(Select ifs such that Uses(ifs, \"x\")))";

				QueryObject expectedQo2a;
				expectedQo2a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo2a.insertClause(Follows, INTEGER, "1", INTEGER, "1", false);

				QueryObject expectedQo2b;
				expectedQo2b.insertSelectStmt(IF, "ifs", NONE);
				expectedQo2b.insertClause(Uses, IF, "ifs", VAR_IDENT, "x", false);

				QueryObject expectedQo2c;
				expectedQo2c.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo2c.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "y", false);

				vqo.clear();
				vqo.push_back(expectedQo2c);
				vqo.push_back(expectedQo2b);
				vqo.push_back(expectedQo2a);

				preprocessor.preprocessQuery(query2);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result2 = Utils::compareQueryObjectProperties(vqo[i], q[0]);
					Assert::AreEqual(true, result2);
				}

				string query3 = "assign a; if ifs; Select a such that Follows(1,(Select ifs such that Uses((Select a such that Modifies(a, \"y\")), \"x\")))";

				QueryObject expectedQo3a;
				expectedQo3a.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo3a.insertClause(Follows, INTEGER, "1", INTEGER, "1", false);

				QueryObject expectedQo3b;
				expectedQo3b.insertSelectStmt(IF, "ifs", NONE);
				expectedQo3b.insertClause(Uses, INTEGER, "1", VAR_IDENT, "x", false);

				QueryObject expectedQo3c;
				expectedQo3c.insertSelectStmt(ASSIGN, "a", NONE);
				expectedQo3c.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "y", false);

				vqo.clear();
				vqo.push_back(expectedQo3c);
				vqo.push_back(expectedQo3b);
				vqo.push_back(expectedQo3a);

				preprocessor.preprocessQuery(query3);
				vqc = preprocessor.getQueryContent();
				queryQueuer.setQueryContent(vqc);
				for (int i = 0; i < vqc.size(); i++) {
					std::vector<QueryObject> q = queryQueuer.parseQueryContent(vqc[i]);
					bool result3 = Utils::compareQueryObjectProperties(vqo[i], q[0]);
					Assert::AreEqual(true, result3);
				}

			}
	};
}