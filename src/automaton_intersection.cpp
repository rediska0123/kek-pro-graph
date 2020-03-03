#include "automaton.h"
#include <map>
#include <deque>

using namespace std;

NonDeterministicAutomaton intersect(
	const DeterministicAutomaton &a,
	const NonDeterministicAutomaton &b
) {
	// bfs on pairs: (first automaton state, second automaton state)
	
	NonDeterministicAutomaton res;
	// maps bfs pairs to indexes in new result automaton
	map <pair <int, int>, int> old_to_new_id;
	
	pair <int, int> start = make_pair(a.get_start_state(), b.get_start_state());
	old_to_new_id[start] = res.get_start_state();
	
	deque <pair <int, int> > d = {start};
	
	auto add_vertex_if_not_exists =
	[&res, &old_to_new_id, &d](pair <int, int> v) mutable {
		if (old_to_new_id.count(v) == 0) {
			d.push_back(v);
			old_to_new_id[v] = res.add_state();
		}
	};
	
	while (!d.empty()) {
		pair <int, int> v = d[0];
		d.pop_front();
		for (const pair <char, int> p : a._automaton[v.first].go) { // deterministic edges
			char symb = p.first;
			int to_first = p.second;
			if (b._automaton[v.second].go.count(symb) == 0)
				throw invalid_argument("Automatons are not complete over the same alphabet");
			for (int to_second : b._automaton[v.second].go.at(symb)) { // non-deterministic edges
				pair <int, int> to = make_pair(to_first, to_second);
				add_vertex_if_not_exists(to);
				res.add_edge(old_to_new_id[v], old_to_new_id[to], symb);
			}
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
