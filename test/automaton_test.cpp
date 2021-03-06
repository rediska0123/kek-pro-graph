#include "automaton_test.h"
#include "automaton.h"
#include "regex_to_fsa.h"

using namespace std;

struct edge {
	int from, to;
	char c;
};

NonDeterministicAutomaton construct_automaton(
	int states_num,
	vector <edge> edges,
	vector <int> terminals
) {
	NonDeterministicAutomaton a;
	vector <int> s = {a.get_start_state()};
	for (int i = 0; i < states_num - 1; i++)
		s.push_back(a.add_state());
	for (edge e : edges)
		a.add_edge(e.from, e.to, e.c);
	for (int t : terminals)
		a.mark_terminal(t);
	return a;
}

// same as construct_automaton, but with deterministic automaton
DeterministicAutomaton construct_det_automaton(
	int states_num,
	vector <edge> edges,
	vector <int> terminals
) {
	DeterministicAutomaton a;
	vector <int> s = {a.get_start_state()};
	for (int i = 0; i < states_num - 1; i++)
		s.push_back(a.add_state());
	for (edge e : edges)
		a.add_edge(e.from, e.to, e.c);
	for (int t : terminals)
		a.mark_terminal(t);
	return a;
}

// generates automaton for aa*a
NonDeterministicAutomaton test_automaton1() {
	return construct_automaton(3,
		{ { 0, 1, 'a' }, { 1, 1, 'a' }, { 1, 2, 'a' } },
		{ 2 }
	);
}

// generates automaton for b(c|d)*(d|r)
NonDeterministicAutomaton test_automaton2() {
	return construct_automaton(4,
		{ { 0, 1, 'b' }, { 1, 1, 'c' }, { 1, 1, 'd' }, { 1, 2, 'd' }, { 1, 3, 'r' } },
		{ 2, 3 }
	);
}

// generates automaton for (ab|aba|aab)*
NonDeterministicAutomaton test_automaton3() {
	return construct_automaton(9,
		{ { 0, 1, 'a' }, { 1, 2, 'b' }, { 2, 0, '\0' },
		  { 0, 3, 'a' }, { 3, 4, 'b' }, { 4, 5, 'a' }, { 5, 0, '\0' },
		  { 0, 6, 'a' }, { 6, 7, 'a' }, { 7, 8, 'b' }, { 8, 0, '\0' } },
		{ 0 }
	);
}

#define CHECK_ACCEPT(a, accepted, not_accepted)                \
	{                                                          \
		for (string i : accepted) DO_CHECK(a.accepts(i));      \
		for (string i : not_accepted) DO_CHECK(!a.accepts(i)); \
	}

void AutomatonTest::test_deterministic_construction_and_accepts() {
	DeterministicAutomaton a;
	DO_CHECK(a.size() == 1);
	int s = a.get_start_state();
	int x = a.add_state();
	DO_CHECK(a.size() == 2);
	CHECK_ACCEPT(a, (vector <string> {}), (vector <string> {"", "a", "abc"}));
	a.add_edge(s, x, 'a');
	CHECK_ACCEPT(a, (vector <string> {}), (vector <string> {"", "a", "abc"}));
	DO_CHECK(a.get_terminal_states().empty());
	a.mark_terminal(x);
	DO_CHECK(a.get_terminal_states().size() == 1);
	DO_CHECK(a.is_terminal(x) && !a.is_terminal(s));
	CHECK_ACCEPT(a, (vector <string> {"a"}), (vector <string> {"", "abc", "aaa", "ba", "baaag"}));
	a.unmark_terminal(x);
	int y = a.add_state();
	int z = a.add_state();
	DO_CHECK(a.size() == 4);
	a.mark_terminal(y);
	a.mark_terminal(z);
	a.add_edge(x, y, 'b');
	a.add_edge(x, z, 'c');
	DO_CHECK(a.get_terminal_states().size() == 2);
	CHECK_ACCEPT(a, (vector <string> {"ab", "ac"}), (vector <string> {"", "ba", "aa", "a"}));
}

void AutomatonTest::test_nondeterministic_construction_and_accepts() {
	{
		NonDeterministicAutomaton a;
		DO_CHECK(a.size() == 1);
		int s = a.get_start_state();
		int x = a.add_state();
		DO_CHECK(a.size() == 2);
		CHECK_ACCEPT(a, (vector <string> {}), (vector <string> {"", "a", "abc"}));
		a.add_edge(s, x, 'a');
		CHECK_ACCEPT(a, (vector <string> {}), (vector <string> {"", "a", "abc"}));
		DO_CHECK(a.get_terminal_states().empty());
		a.mark_terminal(x);
		DO_CHECK(a.get_terminal_states().size() == 1);
		DO_CHECK(a.is_terminal(x) && !a.is_terminal(s));
		CHECK_ACCEPT(a, (vector <string> {"a"}), (vector <string> {"", "abc", "aaa", "ba", "baaag"}));
		a.unmark_terminal(x);
		int y = a.add_state();
		a.add_edge(x, x, 'a');
		a.add_edge(x, y, 'a');
		a.mark_terminal(y);
		DO_CHECK(a.get_terminal_states().size() == 1);
		CHECK_ACCEPT(a, (vector <string> {"aa", "aaa", "aaaaaa"}), (vector <string> {"", "a", "aab", "dcwe"}));
	} { // (b|c)* with eps-edges
		NonDeterministicAutomaton a = construct_automaton(3,
			{ { 0, 1, 'b' }, { 0, 1, 'c' }, { 1, 0, '\0' },
			  { 0, 2, 'a'}, { 1, 2, 'a' }, { 1, 2, 'b' }, { 1, 2, 'c' },
			  { 2, 2, 'a' }, { 2, 2, 'b' }, { 2, 2, 'c' } },
			{ 0 });
		CHECK_ACCEPT(a, (vector <string> {
			"", "b", "c", "bcbcbcb", "cbcbbcbcbc", "ccccc"
		}), (vector <string> {
			"a", "ca", "bcbcbacbcb"
		}));
	}
}

void AutomatonTest::test_nondeterministic_concatenate() {
	NonDeterministicAutomaton a = test_automaton1();
	a.unmark_terminal(2);
	NonDeterministicAutomaton b = test_automaton2();
	a.concatenate_with(b, 2);
	CHECK_ACCEPT(a, (vector <string> {
		"aabcddcd", "aaabr", "aaaabdcdcr", "aabd"
	}), (vector <string> {
		"abd", "aaaa", "aa", "aaabc", "aaaaacdr"
	}));
}

void AutomatonTest::test_make_deterministic() {
	{ // aa*a
		NonDeterministicAutomaton a = test_automaton1();
		DeterministicAutomaton d = a.make_deterministic();
		CHECK_ACCEPT(d, (vector <string> {
			"aa", "aaa", "aaaaaa"
		}), (vector <string> {
			"", "a", "aba", "dsedrfg"
		}));
	}
	{ // b(c|d)*(d|r)
		NonDeterministicAutomaton a = test_automaton2();
		DeterministicAutomaton d = a.make_deterministic();
		CHECK_ACCEPT(d, (vector <string> {
			"bd", "br", "bcdcd", "bcddr"
		}), (vector <string> {
			"", "dr", "bc", "bcdcdcdcdc"
		}));
	}
	{ // (ab|aba|aab)*
		NonDeterministicAutomaton a = test_automaton3();
		DeterministicAutomaton d = a.make_deterministic();
		CHECK_ACCEPT(d, (vector <string> {
			"", "ab", "aba", "aab", "aabab", "aababaaababaabaababa"
		}), (vector <string> {
			"a", "b", "abb", "dsedrfg", "abbabbabaaaab"
		}));
	}	
}

void AutomatonTest::test_deterministic_minimize() {
	{ // aa*a
		NonDeterministicAutomaton a = test_automaton1();
		DeterministicAutomaton d = a.make_deterministic().minimize();
		CHECK_ACCEPT(d, (vector <string> {
			"aa", "aaa", "aaaaaa"
		}), (vector <string> {
			"", "a", "aba", "dsedrfg"
		}));
		DO_CHECK(d.size() == 3);
	}
	{ // b(c|d)*(d|r)
		NonDeterministicAutomaton a = test_automaton2();
		DeterministicAutomaton d = a.make_deterministic().minimize();
		CHECK_ACCEPT(d, (vector <string> {
			"bd", "br", "bcdcd", "bcddr"
		}), (vector <string> {
			"", "dr", "bc", "bcdcdcdcdc"
		}));
		DO_CHECK(d.size() == 4);
	}
	{ // (ab|aba|aab)*
		NonDeterministicAutomaton a = test_automaton3();
		DeterministicAutomaton d = a.make_deterministic().minimize();
		CHECK_ACCEPT(d, (vector <string> {
			"", "ab", "aba", "aab", "aabab", "aababaaababaabaababa"
		}), (vector <string> {
			"a", /*"b",*/ "abb", "dsedrfg", "abbabbabaaaab"
		})); 
		DO_CHECK(d.size() == 5);
	}
	{ // check if minimize() deletes unreachable vertexes
		DeterministicAutomaton a;
		int s = a.get_start_state();
		int x = a.add_state();
		a.add_state(); // unreachable state
		a.add_edge(s, x, 'a');
		a.mark_terminal(x);
		DeterministicAutomaton d = a.minimize();
		CHECK_ACCEPT(d, (vector <string> {
			"a"
		}), (vector <string> {
			"", "aa", "baa", "aaa"
		}));
		DO_CHECK(d.size() == 2);
	}
	{
		DeterministicAutomaton a = regex_to_dfsa("(a|b)(a|b)*(aa|bb|abab|baba)*(a|b)(a|b)*"); // equal to (a|b)(a|b)(a|b)*
		CHECK_ACCEPT(a, (vector <string> {
			"abababbaabaababababbaa", "aaaa", "abba", "aababb", "ab", "aa"
		}), (vector <string> {
			"", "a", "c", "abc", "b"
		}));
		DO_CHECK(a.size() == 3);
	}
	{
		DeterministicAutomaton a = regex_to_dfsa("(a|b)(a|b)*(aa|bb|abab|baba)(a|b)(a|b)*");
		CHECK_ACCEPT(a, (vector <string> {
			"abababbaabaababababbaa", "aaaa", "abba", "aababb",
		}), (vector <string> {
			"", "a", "c", "abc", "b", "abab"
		}));
		DO_CHECK(a.size() == 9);
	}
}

void AutomatonTest::test_intersection() {
	{ // aaa* and (ab|aba|aaa)* -> (aaa)+
		DeterministicAutomaton a = construct_det_automaton(4,
			{ { 0, 1, 'a' }, { 1, 2, 'a' }, { 2, 2, 'a' },
			  { 0, 3, 'b' }, { 1, 3, 'b' }, { 2, 3, 'b' }, { 3, 3, 'b' } },
			{ 2 });
		NonDeterministicAutomaton b = construct_automaton(7,
			{ { 0, 1, 'a' }, { 1, 0, 'b' },
			  { 0, 2, 'a' }, { 2, 3, 'b' }, { 3, 0, 'a' },
			  { 0, 4, 'a' }, { 4, 5, 'a' }, { 5, 0, 'a' },
			  { 0, 6, 'b' }, { 1, 6, 'a' }, { 2, 6, 'a' }, { 3, 6, 'b' },
			  { 4, 6, 'b' }, { 5, 6, 'b' }, { 6, 6, 'a' }, { 6, 6, 'b' } },
			{ 0 });
		NonDeterministicAutomaton c = intersect(a, b);
		CHECK_ACCEPT(c, (vector <string> {
			"aaa", "aaaaaa", "aaaaaaaaa"
		}), (vector <string> {
			"", "a", "aa", "aba", "ab"
		}));
	} { // a* and a* -> a*
		DeterministicAutomaton a = construct_det_automaton(1,
			{ { 0, 0, 'a' } },
			{ 0 });
		NonDeterministicAutomaton b = construct_automaton(1,
			{ { 0, 0, 'a' } },
			{ 0 });
		NonDeterministicAutomaton c = intersect(a, b);
		CHECK_ACCEPT(c, (vector <string> {
			"", "a", "aaa", "aaaaaa", "aaaaaaaaa"
		}), (vector <string> {
			"aab", "abc"
		})); 
	} { // a* and (aa)* -> (aa)*
		DeterministicAutomaton a = construct_det_automaton(1,
			{ { 0, 0, 'a' } },
			{ 0 });
		NonDeterministicAutomaton b = construct_automaton(2,
			{ { 0, 1, 'a' }, { 1, 0, 'a' } },
			{ 0 });
		NonDeterministicAutomaton c = intersect(a, b);
		CHECK_ACCEPT(c, (vector <string> {
			"", "aa", "aaaa", "aaaaaa", "aaaaaaaa"
		}), (vector <string> {
			"a", "aaa", "aab", "abc"
		})); 
	} { // (a|b)* and (b|c)* -> b*
		DeterministicAutomaton a = construct_det_automaton(2,
			{ { 0, 0, 'a' }, { 0, 0, 'b' },
			  { 0, 1, 'c' }, { 1, 1, 'a' }, { 1, 1, 'b' }, { 1, 1, 'c' } },
			{ 0 });
		NonDeterministicAutomaton b = construct_automaton(3, // (b|c)* very non-deterministic automaton
			{ { 0, 1, 'b' }, { 0, 1, 'c' }, { 1, 0, 'b' }, { 1, 0, 'c' },
			  { 0, 0, 'b' }, { 0, 0, 'c' }, { 1, 1, 'b' }, { 1, 1, 'c' },
			  { 0, 2, 'a' }, { 1, 2, 'a' }, { 2, 2, 'a' }, { 2, 2, 'b' }, { 2, 2, 'c' } },
			{ 0, 1 });
		NonDeterministicAutomaton c = intersect(a, b);
		CHECK_ACCEPT(c, (vector <string> {
			"", "b", "bb", "bbb", "bbbbbbbb"
		}), (vector <string> {
			"a", "c", "bba", "cac", "abc"
		}));
	} { // (aaaa|aaaaaa)* and a*aa*aa*a -> aaaa(aa)*
		DeterministicAutomaton a = construct_automaton(9,
			{ { 0, 1, 'a' }, { 1, 2, 'a' }, { 2, 3, 'a' }, { 3, 0, 'a' },
			  { 0, 4, 'a' }, { 4, 5, 'a' }, { 5, 6, 'a' }, { 6, 7, 'a' }, { 7, 8, 'a' }, { 8, 0, 'a' } },
			{ 0 }).make_deterministic();
		NonDeterministicAutomaton b = construct_automaton(5,
			{ { 0, 0, 'a'}, { 0, 1, 'a' }, { 1, 1, 'a' }, { 1, 2, 'a' }, { 2, 2, 'a' }, { 2, 3, 'a' },
			  { 3, 4, 'a' }, { 4, 4, 'a' } },
			{ 3 });
		NonDeterministicAutomaton c = intersect(a, b);
		CHECK_ACCEPT(c, (vector <string> {
			"aaaa", "aaaaaa", "aaaaaaaa", "aaaaaaaaaa"
		}), (vector <string> {
			"", "a", "aa", "aaa", "aaaaa", "aaaaaaa", "aaaaaaaaaaa"
		}));
	}
}

void AutomatonTest::run_all_tests() {
	printf("Automaton testing...\n");
	test_nondeterministic_construction_and_accepts();
	test_nondeterministic_concatenate();
	test_make_deterministic();
	test_deterministic_construction_and_accepts();
	test_deterministic_minimize();
	test_intersection();
}
