#include "parse_regex_test.h"
#include "parse_regex.h"
#include <algorithm>

using namespace std;

bool eq(TreeNode *x, TreeNode *y) {
	if (x == nullptr && y == nullptr)
		return true;
	if (x == nullptr || y == nullptr)
		return false;
	if (x->x != y->x || x->edges.size() != y->edges.size())
		return false;
	for (int i = 0; i < (int)x->edges.size(); i++)
		if (!eq(x->edges[i], y->edges[i]))
			return false;
	return true;
}

TreeNode *symb_node(char r) { return new TreeNode(r, SYMBOL, {}); }
TreeNode *union_node(TreeNode *l, TreeNode *r) { return new TreeNode('|', UNION, {l, r}); }
TreeNode *concat_node(TreeNode *l, TreeNode *r) { return new TreeNode(' ', CONCAT, {l, r}); }
TreeNode *star_node(TreeNode *x) { return new TreeNode('*', STAR, {x}); }

struct test {
	string regex;
	bool is_correct;
	TreeNode *res;
};

void ParseRegexTest::test_general() {
	const vector <test> tests = {
		{ "", true, nullptr },
		{ "()", true, nullptr },
		{ "s", true, symb_node('s') },
		{ "\\(", true, symb_node('(') },
		{ "\\)", true, symb_node(')') },
		{ "\\*", true, symb_node('*') },
		{ "\\|", true, symb_node('|') },
		{ "a|b", true, union_node(symb_node('a'), symb_node('b')) },
		{ "ab", true, concat_node(symb_node('a'), symb_node('b')) },
		{ "a*", true, star_node(symb_node('a')) },
		{ "\\)*", true, star_node(symb_node(')')) },
		{ "a*|b*", true, union_node(star_node(symb_node('a')), star_node(symb_node('b'))) },
		{ "(a|b)(c|d)", true, concat_node(
			union_node(symb_node('a'), symb_node('b')),
			union_node(symb_node('c'), symb_node('d'))) },
		{ "(a|b)c|d", true, union_node(
			concat_node(union_node(symb_node('a'), symb_node('b')), symb_node('c')),
			symb_node('d')) },
		{ "a|bc|d", true, union_node(
			symb_node('a'),
			union_node(concat_node(symb_node('b'), symb_node('c')), symb_node('d'))) },
		{ "a|b(c|d)", true, union_node(
			symb_node('a'),
			concat_node(symb_node('b'), union_node(symb_node('c'), symb_node('d')))) },
		{ "ab*", true, concat_node(
			symb_node('a'),
			star_node(symb_node('b'))) },
		{ "ab*", true, concat_node(
			symb_node('a'),
			star_node(symb_node('b'))) },
		{ "(a|b*|c)*b(zx)(a)", true,
			concat_node(
				star_node(
					union_node(
						symb_node('a'),
						union_node(
							star_node(symb_node('b')),
							symb_node('c')))),
				concat_node(
					symb_node('b'),
					concat_node(
						concat_node(symb_node('z'), symb_node('x')),
						symb_node('a')))) },
		{ "\\|\\*\\(\\)", true,
			concat_node(
				symb_node('|'),
				concat_node(
					symb_node('*'),
					concat_node(
						symb_node('('),
						symb_node(')')))) },
		// incorrect regex syntax tests
		{ "abc(de", false, nullptr },
		{ "())", false, nullptr },
		{ "\\a", false, nullptr },
		{ "(*)", false, nullptr },
		{ "|b", false, nullptr },
		{ "b|", false, nullptr },
	};
	for (test t: tests) {
		TreeNode *got;
		try {
			got = parse_regex(t.regex);
		} catch (...) {
			DO_CHECK(!t.is_correct);
			continue;
		}
		DO_CHECK(t.is_correct);
		DO_CHECK(eq(got, t.res));
	}
}

void ParseRegexTest::run_all_tests() {
	printf("ParseRegex testing...\n");
	test_general();
}
