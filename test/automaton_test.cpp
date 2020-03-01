#include "automaton_test.h"
#include "automaton.h"

using namespace std;

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
}

// generates automaton for aa*a
NonDeterministicAutomaton test_automaton1() {
	NonDeterministicAutomaton a;
	int s = a.get_start_state();
	int x = a.add_state();
	int y = a.add_state();
	a.add_edge(s, x, 'a');
	a.add_edge(x, x, 'a');
	a.add_edge(x, y, 'a');
	a.mark_terminal(y);
	return a;
}

// generates automaton for b(c|d)*(d|r)
NonDeterministicAutomaton test_automaton2() {
	NonDeterministicAutomaton a;
	int s = a.get_start_state();
	int x = a.add_state();
	int y = a.add_state();
	int z = a.add_state();
	a.add_edge(s, x, 'b');
	a.add_edge(x, x, 'c');
	a.add_edge(x, x, 'd');
	a.add_edge(x, y, 'd');
	a.add_edge(x, z, 'r');
	a.mark_terminal(y);
	a.mark_terminal(z);
	return a;
}

// generates automaton for (ab|aba|aab)*
NonDeterministicAutomaton test_automaton3() {
	NonDeterministicAutomaton a;
	int s = a.get_start_state();
	// ab
	int x = a.add_state();
	int y = a.add_state();
	a.add_edge(s, x, 'a');
	a.add_edge(x, y, 'b');
	a.add_eps_edge(y, s);
	// aba
	x = a.add_state();
	y = a.add_state();
	int z = a.add_state();
	a.add_edge(s, x, 'a');
	a.add_edge(x, y, 'b');
	a.add_edge(y, z, 'a');
	a.add_eps_edge(z, s);
	// aab
	x = a.add_state();
	y = a.add_state();
	z = a.add_state();
	a.add_edge(s, x, 'a');
	a.add_edge(x, y, 'a');
	a.add_edge(y, z, 'b');
	a.add_eps_edge(z, s);
	a.mark_terminal(s);
	return a;
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
			"a", "b", "abb", "dsedrfg", "abbabbabaaaab"
		}));
		DO_CHECK(d.size() == 5);
	}
}

void AutomatonTest::run_all_tests() {
	printf("Automaton testing...\n");
	test_nondeterministic_construction_and_accepts();
	test_nondeterministic_concatenate();
	test_make_deterministic();
	test_deterministic_construction_and_accepts();
	test_deterministic_minimize();
}
