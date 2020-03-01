#include "regex_to_fsa.h"
#include "parse_regex.h"
#include <cassert>

using namespace std;

NonDeterministicAutomaton _regextree_to_eps_fsa(TreeNode *tr) {
	if (tr == nullptr) {
		NonDeterministicAutomaton a;
		a.mark_terminal(a.get_start_state());
		return a;
	}
	if (tr->type == SYMBOL) {
		NonDeterministicAutomaton a;
		int x = a.get_start_state();
		int y = a.add_state();
		a.mark_terminal(y);
		a.add_edge(x, y, tr->x);
		return a;
	}
	if (tr->type == UNION) {
		NonDeterministicAutomaton a;
		int s = a.get_start_state();
		int x = a.add_state();
		int y = a.add_state();
		a.add_eps_edge(s, x);
		a.add_eps_edge(s, y);
		a.concatenate_with(_regextree_to_eps_fsa(tr->edges[0]), x);
		a.concatenate_with(_regextree_to_eps_fsa(tr->edges[1]), y);
		return a;
	}
	if (tr->type == STAR) {
		NonDeterministicAutomaton a = _regextree_to_eps_fsa(tr->edges[0]);
		vector <int> t = a.get_terminal_states();
		int s = a.get_start_state();
		for (int x : t)
			if (x != s) {
				a.add_eps_edge(x, s);
				a.unmark_terminal(x);
			}
		a.mark_terminal(s);
		return a;
	}
	if (tr->type == CONCAT) {
		NonDeterministicAutomaton a = _regextree_to_eps_fsa(tr->edges[0]);
		NonDeterministicAutomaton b = _regextree_to_eps_fsa(tr->edges[1]);
		vector <int> t = a.get_terminal_states();
		int st = a.add_state();
		for (int x : t) {
			a.add_eps_edge(x, st);
			a.unmark_terminal(x);
		}
		a.concatenate_with(b, st);
		return a;
	}
	assert(false);
}

DeterministicAutomaton regex_to_dfsa(const string &regex) {
	TreeNode *tr = parse_regex(regex);
	return _regextree_to_eps_fsa(tr).make_deterministic().minimize();
}
