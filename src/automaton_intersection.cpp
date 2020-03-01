#include "automaton.h"
#include <map>
#include <deque>

using namespace std;

DeterministicAutomaton intersect(const DeterministicAutomaton &a, const DeterministicAutomaton &b) {
	map <pair <int, int>, int> old_to_new_id;
	pair <int, int> start = make_pair(a.get_start_state(), b.get_start_state());
	DeterministicAutomaton res;
	old_to_new_id[start] = res.get_start_state();
	if (a.is_terminal(start.first) && b.is_terminal(start.second))
		res.mark_terminal(old_to_new_id[start]);
	
	deque <pair <int, int> > d = {start};
	
	while (!d.empty()) {
		pair <int, int> v = d[0];
		d.pop_front();
		for (const pair <char, int> p : a._automaton[v.first].go) {
			if (b._automaton[v.second].go.count(p.first) == 0)
				throw invalid_argument("Given automatons are not complete");
			const pair <int, int> &to = make_pair(p.second, b._automaton[v.second].go.at(p.first));
			if (old_to_new_id.count(to) == 0) {
				d.push_back(to);
				old_to_new_id[to] = res.add_state();
				if (a.is_terminal(to.first) && b.is_terminal(to.second))
					res.mark_terminal(old_to_new_id[to]);
			}
			res.add_edge(old_to_new_id[v], old_to_new_id[to], p.first);
		}
	}
	
	return res;
}

DeterministicAutomaton intersect(const NonDeterministicAutomaton &a, const NonDeterministicAutomaton &b) {
	return intersect(a.make_deterministic(), b.make_deterministic());
}
