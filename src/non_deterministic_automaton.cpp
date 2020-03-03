#include "automaton.h"

#include <set>
#include <deque>
#include <unordered_map>
#include <algorithm>

using namespace std;

NonDeterministicAutomaton::NonDeterministicAutomaton() { _start = add_state(); }

int NonDeterministicAutomaton::add_state() {
	_automaton.push_back(Vertex());
	return (int)_automaton.size() - 1;
}

void NonDeterministicAutomaton::mark_terminal(int st) { _automaton[st].is_terminal = true; }

void NonDeterministicAutomaton::unmark_terminal(int st) { _automaton[st].is_terminal = false; }

void NonDeterministicAutomaton::add_edge(int from_st, int to_st, char s) { _automaton[from_st].go[s].push_back(to_st); }

void NonDeterministicAutomaton::add_eps_edge(int from_st, int to_st) { add_edge(from_st, to_st, EPS_EDGE_SYMBOL); }

void NonDeterministicAutomaton::concatenate_with(const NonDeterministicAutomaton &a, int st) {
	map <int, int> old_to_new_id;
	old_to_new_id[a.get_start_state()] = st;
	
	for (int i = 0; i < (int)a._automaton.size(); i++) {
		if (i != a.get_start_state())
			old_to_new_id[i] = add_state();
		if (a.is_terminal(i))
			mark_terminal(old_to_new_id[i]);
	}
	
	for (int i = 0; i < (int)a._automaton.size(); i++)
		for (const pair <char, vector <int>> &p : a._automaton[i].go)
			for (int y : p.second) {
				// i -> y, p.first
				int from = old_to_new_id[i];
				int to = old_to_new_id[y];
				char c = p.first;
				_automaton[from].go[c].push_back(to);
			}
}

int NonDeterministicAutomaton::get_start_state() const { return _start; }

bool NonDeterministicAutomaton::is_terminal(int st) const { return _automaton[st].is_terminal; }

vector <int> NonDeterministicAutomaton::get_terminal_states() const {
	vector <int> ans;
	for (int i = 0; i < (int)_automaton.size(); i++)
		if (_automaton[i].is_terminal)
			ans.push_back(i);
	return ans;
}

size_t NonDeterministicAutomaton::size() const { return _automaton.size(); }

bool NonDeterministicAutomaton::_accepts_from_vertex(int st, const string &s, int i) {
	if (i >= (int)s.length())
		return _automaton[st].is_terminal;
	vector <pair <char, int> > nxt;
	_step_once({st}, nxt);
	for (pair <char, int> x : nxt)
		if (x.first == s[i] && _accepts_from_vertex(x.second, s, i + 1))
			return true;
	return false;
}

bool NonDeterministicAutomaton::accepts(const string &s) { return _accepts_from_vertex(_start, s); }

void NonDeterministicAutomaton::_step_eps(const vector <int> &st, vector <int> &res) const {
	// bfs on eps-edges

	unordered_map <int, bool> used;
	deque <int> d(st.begin(), st.end());
	for (int x : st) {
		used[x] = true;
		res.push_back(x);
	}
	while (!d.empty()) {
		int x = d[0];
		d.pop_front();
		for (const pair <char, vector <int> > &p : _automaton[x].go)
			if (p.first == EPS_EDGE_SYMBOL)
				for (int to : p.second)
					if (!used[to]) {
						used[to] = true;
						res.push_back(to);
						d.push_back(to);
					}
	}
}

void NonDeterministicAutomaton::_step_once(const vector <int> &st, vector <pair <char, int> > &res) const {
	// Alrotighm:
	//    cmp    <- _step_eps from st
	//    to     <- all endings of non-eps edges leading from cmp
	//    to_cmp <- _step_eps from to
	// to_cmp --- answer

	vector <int> cmp;
	_step_eps(st, cmp);
	map <pair <int, int>, bool> used;
	
	for (int v : cmp)
		for (const pair <char, vector <int> > &p : _automaton[v].go)
			// find non-eps edges
			if (p.first != EPS_EDGE_SYMBOL)
				for (int to : p.second) {
					vector <int> to_cmp;
					_step_eps({to}, to_cmp);
					for (int y : to_cmp) {
						pair <char, int> new_edge = make_pair(p.first, y);
						if (!used[new_edge]) {
							res.push_back(new_edge);
							used[new_edge] = true;
						}
					}
				}
}

DeterministicAutomaton NonDeterministicAutomaton::make_deterministic() const {
	// bfs on sets of current automaton vertexes
	
	vector <int> start_set;
	_step_eps({_start}, start_set);
	
	// a set to starat with
	set <int> sset(start_set.begin(), start_set.end());
	
	// maps sets to a new automaton vertex ids
	map <set <int>, int> new_automaton_id;
	
	DeterministicAutomaton res;
	new_automaton_id[sset] = res.get_start_state();
	
	deque <set <int> > d = {sset};
	
	while (!d.empty()) {
		set <int> vset = d[0];
		d.pop_front();
		
		vector <pair <char, int> > nxt;
		_step_once(vector <int> (vset.begin(), vset.end()), nxt);
		
		// group all edges by symbol in 'go'
		map <char, set <int> > go;
		for (pair <char, int> &p : nxt)
			go[p.first].insert(p.second);
		
		for (const pair <char, set <int> > &p : go) {
			char symb = p.first;
			const set <int> &toset = p.second;
			if (new_automaton_id.count(toset) == 0) {
				new_automaton_id[toset] = res.add_state();
				d.push_back(toset);
			}
			res.add_edge(new_automaton_id[vset], new_automaton_id[toset], symb);
		}
	}
	
	// mark terminal vertexes
	for (const pair <set <int>, int> &p : new_automaton_id) {
		bool terminal = false;
		for (int v : p.first)
			if (is_terminal(v)) {
				terminal = true;
				break;
			}
		if (terminal)
			res.mark_terminal(p.second);
	}
	
	return res;
}
