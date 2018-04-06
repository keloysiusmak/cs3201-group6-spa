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
				Assert::AreEqual(true, compareQueryContentProperties(vqc[i], qq.getQueryContent()[i]));
			}
		}

		TEST_METHOD(QueryQueuerParseQueryContent)
		{
			PKB pkb;
			QueryQueuer qq;

			QueryContent qc;
			qc.insertClause(Modifies, VARIABLE, "v", IDENT, "x");
			qc.insertClause(Uses, VARIABLE, "v", IDENT, "y");
			qc.insertOperator(CLAUSE, OR);

			std::vector<QueryContent> vqc;
			vqc.push_back(qc);
			qq.setQueryContent(vqc);
			for (int i = 0; i < vqc.size(); i++) {
				std::vector<QueryObject> result = qq.parseQueryContent(vqc[i]);
				QueryObject expectedQo1;
				expectedQo1.insertClause(Modifies, VARIABLE, "v", IDENT, "x");
				QueryObject expectedQo2;
				expectedQo2.insertClause(Uses, VARIABLE, "v", IDENT, "y");
				Assert::AreEqual(true, compareQueryObjectProperties(result[0], expectedQo1));
				Assert::AreEqual(true, compareQueryObjectProperties(result[1], expectedQo2));
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
			Clause c = Clause(Modifies, lp, rp);
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
			expectedClause1.setClause(Modifies, lp, rp);
			Clause expectedClause2;
			expectedClause2.setClause(Modifies, lp, rp);
			Clause expectedClause3;
			expectedClause3.setClause(Modifies, lp, rp);
			Assert::AreEqual(true, compareClause(expectedClause1, clause[0]));
			Assert::AreEqual(true, compareClause(expectedClause2, clause[1]));
			Assert::AreEqual(true, compareClause(expectedClause3, clause[2]));
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

			Pattern c = Pattern(lp, rp, rrp);
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
			expectedPattern1.setPattern(lp, rp, rrp);
			Pattern expectedPattern2;
			expectedPattern2.setPattern(lp, rp, rrp);
			Pattern expectedPattern3;
			expectedPattern3.setPattern(lp, rp, rrp);
			Assert::AreEqual(true, comparePattern(expectedPattern1, patterns[0]));
			Assert::AreEqual(true, comparePattern(expectedPattern2, patterns[1]));
			Assert::AreEqual(true, comparePattern(expectedPattern3, patterns[2]));
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
			Clause c = Clause(With, lp, rp);
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
			expectedClause1.setClause(With, lp, rp);
			Clause expectedClause2;
			expectedClause2.setClause(With, lp, rp);
			Clause expectedClause3;
			expectedClause3.setClause(With, lp, rp);
			Assert::AreEqual(true, compareClause(expectedClause1, clause[0]));
			Assert::AreEqual(true, compareClause(expectedClause2, clause[1]));
			Assert::AreEqual(true, compareClause(expectedClause3, clause[2]));
		}

		bool compareClauses(QueryObject qo1, QueryObject qo2) {
			bool isSameClauses = qo1.getClauses().size() == qo2.getClauses().size();

			if (isSameClauses) {
				for (size_t i = 0; i < qo1.getClauses().size(); i++) {
					Clause c1 = qo1.getClauses().at(i);
					Clause c2 = qo2.getClauses().at(i);

					isSameClauses = compareClause(c1, c2);
				}
			}
			return isSameClauses;
		}
		bool compareClause(Clause c1, Clause c2) {
			if (c1.getRelRef() != c2.getRelRef() ||
				c1.getLeftParam().type != c2.getLeftParam().type ||
				c1.getLeftParam().value.compare(c2.getLeftParam().value) != 0 ||
				c1.getLeftParam().attribute != c2.getLeftParam().attribute ||
				c1.getRightParam().type != c2.getRightParam().type ||
				c1.getRightParam().value.compare(c2.getRightParam().value) != 0 ||
				c1.getRightParam().attribute != c2.getRightParam().attribute) {
				return false;
			}
			return true;
		}
		bool comparePatterns(QueryObject qo1, QueryObject qo2) {
			bool isSamePatterns = qo1.getPatterns().size() == qo2.getPatterns().size();

			if (isSamePatterns) {
				for (size_t i = 0; i < qo1.getPatterns().size(); i++) {
					Pattern p1 = qo1.getPatterns().at(i);
					Pattern p2 = qo2.getPatterns().at(i);

					isSamePatterns = comparePattern(p1, p2);
				}
			}
			return isSamePatterns;
		}
		bool comparePattern(Pattern p1, Pattern p2) {
			if (p1.getEntity().type != p2.getEntity().type ||
				p1.getEntity().value.compare(p2.getEntity().value) != 0 ||
				p1.getLeftParam().type != p2.getLeftParam().type ||
				p1.getLeftParam().value.compare(p2.getLeftParam().value) != 0 ||
				p1.getLeftParam().attribute != p2.getLeftParam().attribute ||
				p1.getRightParam().type != p2.getRightParam().type ||
				p1.getRightParam().value.compare(p2.getRightParam().value) != 0 ||
				p1.getRightParam().attribute != p2.getRightParam().attribute) {
				return false;
			}
			return true;
		}
		bool compareWithClauses(QueryObject qo1, QueryObject qo2) {
			bool isSameWithClauses = qo1.getWithClauses().size() == qo2.getWithClauses().size();

			if (isSameWithClauses) {
				for (size_t i = 0; i < qo1.getWithClauses().size(); i++) {
					Clause w1 = qo1.getWithClauses().at(i);
					Clause w2 = qo2.getWithClauses().at(i);

					isSameWithClauses = compareWithClause(w1, w2);
				}
			}
			return isSameWithClauses;
		}
		bool compareWithClause(Clause w1, Clause w2) {
			if (w1.getRelRef() != w2.getRelRef() ||
				w1.getLeftParam().type != w2.getLeftParam().type ||
				w1.getLeftParam().value.compare(w2.getLeftParam().value) != 0 ||
				w1.getLeftParam().attribute != w2.getLeftParam().attribute ||
				w1.getRightParam().type != w2.getRightParam().type ||
				w1.getRightParam().value.compare(w2.getRightParam().value) != 0 ||
				w1.getRightParam().attribute != w2.getRightParam().attribute) {
				return false;
			}
			return true;
		}
		bool compareParams(QueryObject qo1, QueryObject qo2) {
			bool isSameSelectStatement = qo1.getSelectStatements().size() == qo2.getSelectStatements().size();

			if (isSameSelectStatement) {
				for (size_t i = 0; i < qo1.getSelectStatements().size(); i++) {
					Param s1 = qo1.getSelectStatements().at(i);
					Param s2 = qo1.getSelectStatements().at(i);

					isSameSelectStatement = compareParam(s1, s2);
				}
			}
			return isSameSelectStatement;
		}
		bool compareParam(Param s1, Param s2) {
			if (s1.type != s2.type ||
				s1.value.compare(s2.value) != 0 ||
				s1.attribute != s2.attribute) {
				return false;
			}
			return true;
		}
		bool compareQueryObjectProperties(QueryObject qo1, QueryObject qo2) {

			bool isSameSelectStatement = compareParams(qo1, qo2);

			bool isSameClauses = compareClauses(qo1, qo2);

			bool isSamePatterns = comparePatterns(qo1, qo2);

			bool isSameWithClauses = compareWithClauses(qo1, qo2);

			return isSameSelectStatement && isSameClauses && isSamePatterns && isSameWithClauses;
		}
		bool compareQueryContentProperties(QueryContent qc1, QueryContent qc2) {
			if (qc1.getSelect().size() != qc2.getSelect().size()) return false;
			if (qc1.getClauses().size() != qc2.getClauses().size()) return false;
			if (qc1.getPattern().size() != qc2.getPattern().size()) return false;
			if (qc1.getWithClauses().size() != qc2.getWithClauses().size()) return false;
			vector<Param> pr1 = qc1.getSelect();
			vector<Param> pr2 = qc2.getSelect();
			for (int i = 0; i < pr1.size(); i++) {
				if (!compareParam(pr1[i], pr2[i])) return false;
			}
			vector<ClauseNode> c1 = qc1.getClauses();
			vector<ClauseNode> c2 = qc2.getClauses();
			for (int i = 0; i < c1.size(); i++) {
				if (!compareClauseNode(c1[i], c2[i])) return false;
			}
			vector<ClauseNode> p1 = qc1.getPattern();
			vector<ClauseNode> p2 = qc2.getPattern();
			for (int i = 0; i < p1.size(); i++) {
				if (!compareClauseNode(p1[i], p2[i])) return false;
			}
			vector<ClauseNode> w1 = qc1.getWithClauses();
			vector<ClauseNode> w2 = qc2.getWithClauses();
			for (int i = 0; i < w1.size(); i++) {
				if (!compareClauseNode(w1[i], w2[i])) return false;
			}

			return true;
		}
		bool compareClauseNode(ClauseNode cn1, ClauseNode cn2) {
			if (cn1.getChildren().size() != cn2.getChildren().size()) return false;
			for (int i = 0; i < cn1.getChildren().size(); i++) {
				if (!compareClauseNode(cn1.getChildren()[i], cn2.getChildren()[i])) return false;
			}
			if (cn1.getClauseNodeType() != cn2.getClauseNodeType()) return false;
			if (!compareClause(cn1.getClause(), cn2.getClause())) return false;
			if (!compareClause(cn1.getPattern(), cn2.getPattern())) return false;
			if (!compareClause(cn1.getWithClause(), cn2.getWithClause())) return false;

			return true;
		}


	};

}