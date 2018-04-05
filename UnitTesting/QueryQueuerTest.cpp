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
			expectedQo2.insertClause(Uses, VARIABLE, "v", IDENT, "y");
			Assert::AreEqual(true, compareQueryObjectProperties(result[0], expectedQo1));
			Assert::AreEqual(true, compareQueryObjectProperties(result[1], expectedQo2));

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
			expectedClause1.setClause(Modifies, lp, rp);
			Clause expectedClause2;
			expectedClause2.setClause(Modifies, lp, rp);
			Clause expectedClause3;
			expectedClause3.setClause(Modifies, lp, rp);
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

			bool isSameClauses = compareClauses(qo1, qo2);

			bool isSamePatterns = comparePatterns(qo1, qo2);

			bool isSameWithClauses = compareWithClauses(qo1, qo2);

			return isSameSelectStatement && isSameClauses && isSamePatterns && isSameWithClauses;
		}


	};

}