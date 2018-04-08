#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/Pattern.h"
#include "../SPA/QueryQueuer.h";
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace QueryQueuerTesting
{
	TEST_CLASS(QueryQueuerTest)
	{
	public:
		
		TEST_METHOD(QueryQueuerIsValidQuery)
		{
			QueryQueuer qq;

			std::vector<QueryContent> vqc;
			QueryContent qc;
			vqc.push_back(qc);
			for (int i = 0; i < vqc.size(); i++) {
				qq.setQueryContent(vqc);
				Assert::AreEqual(true, qq.isValidQuery());
				qq.setInvalidQuery("message");
				Assert::AreEqual(false, qq.isValidQuery());
			}
		}

		TEST_METHOD(QueryQueuerGetQueryContent)
		{
			QueryQueuer qq;

			std::vector<QueryContent> vqc;
			qq.setQueryContent(vqc);
			QueryContent qc;

			for (int i = 0; i < vqc.size(); i++) {
				Assert::AreEqual(true, Utils::compareQueryContentProperties(vqc[i], qq.getQueryContent()[i]));
			}
		}

		TEST_METHOD(QueryQueuerParseQueryContent)
		{
			PKB pkb;
			QueryQueuer qq;

			QueryContent qc;
			qc.insertClause(Modifies, VARIABLE, "v", IDENT, "x", false);
			qc.insertClause(Uses, VARIABLE, "v", IDENT, "y", false);
			qc.insertOperator(CLAUSE, OR);

			std::vector<QueryContent> vqc;
			vqc.push_back(qc);
			qq.setQueryContent(vqc);
			for (int i = 0; i < vqc.size(); i++) {
				std::vector<QueryObject> result = qq.parseQueryContent(vqc[i]);
				QueryObject expectedQo1a;
				expectedQo1a.insertClause(Modifies, VARIABLE, "v", IDENT, "x", false);
				QueryObject expectedQo1b;
				expectedQo1b.insertClause(Uses, VARIABLE, "v", IDENT, "y", false);
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[0], expectedQo1a));
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[1], expectedQo1b));
			}

			QueryContent qc1;
			qc1.insertClause(Modifies, VARIABLE, "v", IDENT, "x", false);
			qc1.insertClause(Uses, VARIABLE, "v", IDENT, "y", false);
			qc1.insertOperator(CLAUSE, OR);
			qc1.insertClause(Modifies, VARIABLE, "v", IDENT, "a", false);
			qc1.insertClause(Uses, VARIABLE, "v", IDENT, "b", false);
			qc1.insertOperator(CLAUSE, OR);
			qc1.insertOperator(CLAUSE, AND);

			std::vector<QueryContent> vqc1;
			vqc1.push_back(qc1);
			qq.setQueryContent(vqc1);
			for (int i = 0; i < vqc1.size(); i++) {
				std::vector<QueryObject> result = qq.parseQueryContent(vqc1[i]);
				QueryObject expectedQo2a;
				expectedQo2a.insertClause(Modifies, VARIABLE, "v", IDENT, "x", false);
				expectedQo2a.insertClause(Modifies, VARIABLE, "v", IDENT, "a", false);
				QueryObject expectedQo2b;
				expectedQo2b.insertClause(Modifies, VARIABLE, "v", IDENT, "x", false);
				expectedQo2b.insertClause(Uses, VARIABLE, "v", IDENT, "b", false);
				QueryObject expectedQo2c;
				expectedQo2c.insertClause(Uses, VARIABLE, "v", IDENT, "y", false);
				expectedQo2c.insertClause(Modifies, VARIABLE, "v", IDENT, "a", false);
				QueryObject expectedQo2d;
				expectedQo2d.insertClause(Uses, VARIABLE, "v", IDENT, "y", false);
				expectedQo2d.insertClause(Uses, VARIABLE, "v", IDENT, "b", false);
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[0], expectedQo2a));
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[1], expectedQo2b));
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[2], expectedQo2c));
				Assert::AreEqual(true, Utils::compareQueryObjectProperties(result[3], expectedQo2d));
			}
		}

		TEST_METHOD(QueryQueuerParseClauseTree)
		{
			PKB pkb;
			QueryQueuer qq;

			std::vector<ClauseNode> clauses;
			ClauseNode c1;
			ClauseNode c2;
			ClauseNode c3;
			ClauseNode c4;
			ClauseNode c5;

			Param lp;
			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			Param rp;
			rp.type = IDENT;
			rp.value = "x";
			rp.attribute = NONE;
			Clause c = Clause(Modifies, lp, rp, false);
			c2.setClauseNode(c);
			c3.setClauseNode(c);
			c5.setClauseNode(c);
			c1.setOperatorNode(AND);
			c1.addChildren(c2);
			c1.addChildren(c3);
			c4.setOperatorNode(AND);
			c4.addChildren(c1);
			c4.addChildren(c5);

			std::vector<Clause> clause = qq.parseClauseTree(c4);
			Clause expectedClause1;
			expectedClause1.setClause(Modifies, lp, rp, false);
			Clause expectedClause2;
			expectedClause2.setClause(Modifies, lp, rp, false);
			Clause expectedClause3;
			expectedClause3.setClause(Modifies, lp, rp, false);
			Assert::AreEqual(true, Utils::compareClause(expectedClause1, clause[0]));
			Assert::AreEqual(true, Utils::compareClause(expectedClause2, clause[1]));
			Assert::AreEqual(true, Utils::compareClause(expectedClause3, clause[2]));
		}

		TEST_METHOD(QueryQueuerParsePatternTree)
		{
			PKB pkb;
			QueryQueuer qq;

			std::vector<ClauseNode> clauses;
			ClauseNode c1;
			ClauseNode c2;
			ClauseNode c3;
			ClauseNode c4;
			ClauseNode c5;

			Param lp;
			lp.type = IF;
			lp.value = "ifs";
			lp.attribute = NONE;

			Param rp;
			rp.type = IDENT;
			rp.value = "x";
			rp.attribute = NONE;

			Param rrp;
			rrp.type = ALL;
			rrp.value = "_";
			rrp.attribute = NONE;

			Pattern c = Pattern(lp, rp, rrp, false);
			c2.setClauseNode(c);
			c3.setClauseNode(c);
			c5.setClauseNode(c);
			c1.setOperatorNode(AND);
			c1.addChildren(c2);
			c1.addChildren(c3);
			c4.setOperatorNode(AND);
			c4.addChildren(c1);
			c4.addChildren(c5);

			std::vector<Pattern> patterns = qq.parsePatternTree(c4);
			Pattern expectedPattern1;
			expectedPattern1.setPattern(lp, rp, rrp, false);
			Pattern expectedPattern2;
			expectedPattern2.setPattern(lp, rp, rrp, false);
			Pattern expectedPattern3;
			expectedPattern3.setPattern(lp, rp, rrp, false);
			Assert::AreEqual(true, Utils::comparePattern(expectedPattern1, patterns[0]));
			Assert::AreEqual(true, Utils::comparePattern(expectedPattern2, patterns[1]));
			Assert::AreEqual(true, Utils::comparePattern(expectedPattern3, patterns[2]));
		}

		TEST_METHOD(QueryQueuerParseWithClauseTree)
		{
			PKB pkb;
			QueryQueuer qq;

			std::vector<ClauseNode> clauses;
			ClauseNode c1;
			ClauseNode c2;
			ClauseNode c3;
			ClauseNode c4;
			ClauseNode c5;

			Param lp;
			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = VARNAME;

			Param rp;
			rp.type = PROCEDURE;
			rp.value = "p";
			rp.attribute = PROCNAME;
			Clause c = Clause(With, lp, rp, false);
			c2.setClauseNode(c);
			c3.setClauseNode(c);
			c5.setClauseNode(c);
			c1.setOperatorNode(AND);
			c1.addChildren(c2);
			c1.addChildren(c3);
			c4.setOperatorNode(AND);
			c4.addChildren(c1);
			c4.addChildren(c5);

			std::vector<Clause> clause = qq.parseClauseTree(c4);
			Clause expectedClause1;
			expectedClause1.setClause(With, lp, rp, false);
			Clause expectedClause2;
			expectedClause2.setClause(With, lp, rp, false);
			Clause expectedClause3;
			expectedClause3.setClause(With, lp, rp, false);
			Assert::AreEqual(true, Utils::compareClause(expectedClause1, clause[0]));
			Assert::AreEqual(true, Utils::compareClause(expectedClause2, clause[1]));
			Assert::AreEqual(true, Utils::compareClause(expectedClause3, clause[2]));
		}


	};

}