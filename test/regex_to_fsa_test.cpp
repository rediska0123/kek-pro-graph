#include "regex_to_fsa_test.h"
#include "regex_to_fsa.h"
#include "automaton.h"

using namespace std;

struct test {
	string regex;
	vector <string> accepted;
	vector <string> non_accepted;
};

void RegexToFsaTest::test_general() {
	string letters = "";
	for (char c = 'a'; c <= 'z'; c++) {
		letters += c;
		if (c != 'z')
			letters += '|';
	}
	
	vector <test> tests = {
		{ "",
			{ "" },
			{ "a", "b", "ab" } },
		{ "abc",
			{ "abc" },
			{ "a", "b", "ab" } },
		{ "(a|b|c)",
			{ "a", "b", "c" }, 
			{ "", "ab", "aaa" } },
		{ "(ab|aab|aba)*",
			{ "", "ab", "aab", "aba", "aababa", "abaababaabaab" },
			{ "a", "baa", "abaababaabaabaa", "aabababb" } },
		{ "a*b*a*",
			{ "", "aba", "a", "ba", "aaaabbbbbaaa", "aaaaaaa" },
			{ "abab", "aaabbbbaaaabb", "aac" } },
		{ "\\)\\*\\(\\|\\)*",
			{ ")*(|)))))", ")*(|" },
			{ ")*|))", "*****" } },
		{ "(a|b)(c|d)e",
			{ "ace", "ade", "bce", "bde" },
			{ "ac", "ae", "aae", "cae" } },
		{ "(a|(b|c)|(e|f)|e)*",
			{ "bbb", "abcef", "bcbcae", "ae" },
			{ "abcdef", "aed", "x" } },
		{ "(" + letters + ")*kek(" + letters + ")*", // any string containing 'kek' as a substring
			{ "vfnrtgdfskekekfddsd", "kek", "efgfdvfkek", "kekvfgf", "aaaaaakekaaa" },
			{ "abdffgh", "ferftgfds", "x", "", "rgthy", "keeeeeee", "kke" } },
	};
	
	for (test t : tests) {
		DeterministicAutomaton d = regex_to_dfsa(t.regex);
		for (string s : t.accepted)
			DO_CHECK(d.accepts(s));
		for (string s : t.non_accepted)
			DO_CHECK(!d.accepts(s));
	}
}

void RegexToFsaTest::run_all_tests() {
	printf("RegexToFsa testing...\n");
	test_general();
}
