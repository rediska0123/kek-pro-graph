#include "automaton.h"

#include <set>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

DeterministicAutomaton::DeterministicAutomaton() {
	_start = add_state();
}

DeterministicAutomaton::DeterministicAutomaton(const DeterministicAutomaton &d) {
	_start = d._start;
	_automaton = d._automaton;
}

int DeterministicAutomaton::add_state() {
	_automaton.push_back(Vertex());
	return (int)_automaton.size() - 1;
}

void DeterministicAutomaton::mark_terminal(int st) { _automaton[st].is_terminal = true; }

void DeterministicAutomaton::unmark_terminal(int st) { _automaton[st].is_terminal = false; }

void DeterministicAutomaton::add_edge(int from_st, int to_st, char s) { _automaton[from_st].go[s] = to_st; }

int DeterministicAutomaton::get_start_state() const { return _start; }

vector <int> DeterministicAutomaton::get_terminal_states() const {
	vector <int> ans;
	for (int i = 0; i < (int)_automaton.size(); i++)
		if (_automaton[i].is_terminal)
			ans.push_back(i);
	return ans;
}

bool DeterministicAutomaton::is_terminal(int st) const { return _automaton[st].is_terminal; }

size_t DeterministicAutomaton::size() const { return _automaton.size(); }

bool DeterministicAutomaton::accepts(const string &s) {
	int x = _start;
	for (char c : s) {
		if (_automaton[x].go.count(c) == 0)
			return false;
		x = _automaton[x].go[c];
	}
	return _automaton[x].is_terminal;
}

void DeterministicAutomaton::print() const {
	cout << size() << " states: " << endl;
	for (int i = 0; i < (int)size(); i++) {
		if (_automaton[i].is_terminal)
			cout << i << ": terminal" << endl;
		else
			cout << i << ": " << endl;
		for (pair <char, int> p : _automaton[i].go)
			cout << "	" << i << " -> " << p.second << " : " << p.first << endl;
	}
}

void DeterministicAutomaton::_complete() {
	int n = size();

	set <char> alphabet;
	// count alphabet : set of all _automaton edges' symbols
	for (int i = 0; i < n; i++)
		for (const pair <char, int> &p : _automaton[i].go)
			alphabet.insert(p.first);
	vector <pair <char, int> > edges_to_add;
	for (int i = 0; i < n; i++)
		for (char c : alphabet)
			if (_automaton[i].go.count(c) == 0)
				edges_to_add.push_back(make_pair(c, i));
	
	// if no edges need to be added do not change the automaton
	if (edges_to_add.empty())
		return;
	
	int sink = add_state();
	for (char c : alphabet)
		add_edge(sink, sink, c);
	
	for (pair <char, int> p : edges_to_add)
		add_edge(p.second, sink, p.first);
}

void DeterministicAutomaton::_delete_unreachable_states() {
	int n = size();
	deque <int> d = {get_start_state()};
	vector <bool> used(n, false);
	used[get_start_state()] = true;
	
	// bfs
	while (!d.empty()) {
		int v = d[0];
		d.pop_front();
		for (const pair <char, int> &p : _automaton[v].go) {
			int to = p.second;
			if (!used[to]) {
				used[to] = true;
				d.push_back(to);
			}
		}
	}

	DeterministicAutomaton res;
	// maps _automaton indexes to new res automaton indexes
	unordered_map <int, int> old_to_new_id;
	old_to_new_id[get_start_state()] = res.get_start_state();
	for (int i = 0; i < n; i++)
		if (i != get_start_state() && used[i])
			old_to_new_id[i] = res.add_state();

	// add edges between reachable states
	for (int i = 0; i < n; i++)
		if (used[i])
			for (const pair <char, int> &p : _automaton[i].go)
				res.add_edge(
					old_to_new_id[i],
					old_to_new_id[p.second],
					p.first);
	
	// mark terminal vertexes
	for (int i = 0; i < n; i++)
		if (used[i] && is_terminal(i))
			res.mark_terminal(old_to_new_id[i]);

	*this = DeterministicAutomaton(res);
}

void DeterministicAutomaton::_delete_state(int st) {
	// change id of the last state (number size() - 1) to st
	
	int n = size();
	if (is_terminal(n - 1))
		mark_terminal(st);
	for (int i = 0; i < n; i++) {
		vector <char> edges_to_change;
		for (const pair <char, int> &p : _automaton[i].go)
			if (p.second == n - 1)
				edges_to_change.push_back(p.first);
		for (int x : edges_to_change)
			_automaton[i].go[x] = st;
	}
	
	_automaton[st] = _automaton[n - 1];
	_automaton.pop_back();
}

DeterministicAutomaton DeterministicAutomaton::minimize() const {
	DeterministicAutomaton d(*this);
	d._delete_unreachable_states();
	d._complete();
	
	int n = d.size();
	vector <vector <bool> > used(n, vector <bool> (n, false));
	
	deque <pair <int, int> > unequal;
	
	// init unequal pairs
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (d.is_terminal(i) != d.is_terminal(j)) {
				unequal.push_back(make_pair(i, j));
				used[i][j] = used[j][i] = true;
			}
	
	// inverse edges from d
	vector <map <char, vector <int> > > inv_a(n);
	for (int i = 0; i < n; i++)
		for (const pair <char, int> &p : d._automaton[i].go)
			inv_a[p.second][p.first].push_back(i);
	
	// find other uneqial pairs in bfs
	while (!unequal.empty()) {
		pair <int, int> v = unequal[0];
		unequal.pop_front();
		for (const pair <char, vector <int> > &p : inv_a[v.first]) {
			char symb = p.first;
			vector <int> to_vertexes = p.second;
			if (inv_a[v.second].count(symb)) {
				for (int x : to_vertexes)
					for (int y : inv_a[v.second][symb])
						if (!used[x][y]) {
							used[x][y] = used[y][x] = true;
							unequal.push_back(make_pair(x, y));
						}
			}
		}
	}

	// construct new automaton from the components
	vector <int> component_root(n, -1);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (!used[i][j]) {
				component_root[i] = j;
				break;
			}

	DeterministicAutomaton res;
	// maps d automaton indexes to a new res automaton indexes
	unordered_map <int, int> old_to_new_id;

	old_to_new_id[d.get_start_state()] = res.get_start_state();
	for (int i = 0; i < n; i++)
		if (i != d.get_start_state() && component_root[i] == i)
			old_to_new_id[i] = res.add_state();
	
	// mark terminal states
	for (pair <int, int> p : old_to_new_id)
		if (d.is_terminal(p.first))
			res.mark_terminal(p.second);

	for (int i = 0; i < n; i++)
		for (const pair <char, int> &p : d._automaton[i].go) {
			// adding the same edge many times is working
			res.add_edge(
				old_to_new_id[component_root[i]],
				old_to_new_id[component_root[p.second]],
				p.first);
		}
	
	// At this point we have minimum complete automaton.
	// To make it just a minimum automaton we need to delete
	// each state which can't lead to the terminal state.
	// After the minimization there is only one (sink) or less such states.
	
	// find sink vertex
	
	int sink = -1;
	for (int i = 0; i < (int)res._automaton.size(); i++) {
		bool is_sink = true;
		for (const pair <char, int> &p : res._automaton[i].go)
			if (p.second != i) {
				is_sink = false;
				break;
			}
		if (is_sink && !res.is_terminal(i))
			sink = i;
	}
	
	if (sink != -1)
		res._delete_state(sink);
	
	return res;
}
