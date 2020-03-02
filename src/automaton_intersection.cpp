#include "automaton.h"
#include <map>
#include <deque>

using namespace std;

DeterministicAutomaton intersect(const DeterministicAutomaton &a, const DeterministicAutomaton &b) {
	// bfs on pairs: (first automaton state, second automaton state)
	
	DeterministicAutomaton res;
	// maps bfs pairs to indexes in new result automaton
	map <pair <int, int>, int> old_to_new_id;
	
	pair <int, int> start = make_pair(a.get_start_state(), b.get_start_state());
	old_to_new_id[start] = res.get_start_state();
	
	deque <pair <int, int> > d = {start};
	
	while (!d.empty()) {
		pair <int, int> v = d[0];
		d.pop_front();
		for (const pair <char, int> p : a._automaton[v.first].go) {
			char symb = p.first;
			if (b._automaton[v.second].go.count(symb) == 0)
				throw invalid_argument("Automatons are not complete");
			const pair <int, int> &to = make_pair(p.second, b._automaton[v.second].go.at(symb));
			if (old_to_new_id.count(to) == 0) {
				d.push_back(to);
				old_to_new_id[to] = res.add_state();
			}
			res.add_edge(old_to_new_id[v], old_to_new_id[to], p.first);
		}
	}
	
	// mark terminal states
	for (const auto &p : old_to_new_id) {
		pair <int, int> x = p.first;
		int xid = p.second;
		if (a.is_terminal(x.first) && b.is_terminal(x.second))
			res.mark_terminal(xid);
	}
	
	return res;
}

DeterministicAutomaton intersect(const NonDeterministicAutomaton &a, const NonDeterministicAutomaton &b) {
	return intersect(a.make_deterministic(), b.make_deterministic());
}
