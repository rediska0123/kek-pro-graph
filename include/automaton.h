#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <string>
#include <map>

// DeterministicAutomaton stores a deterministic finite state machines.
class DeterministicAutomaton {
  public:
	// Creates an automaton with one start state and no terminal states.
	DeterministicAutomaton();
	
	// Constructs an automaton from a.
	DeterministicAutomaton(const DeterministicAutomaton &a);
	
	// Creates a new state and returns its identificator.
	int add_state();
	
	// Mark and unmark a state with given id terminal.
	void mark_terminal(int st);
	void unmark_terminal(int st);

	// Adds an edge with symbol s between states with id from_st and to_st.
	void add_edge(int from_st, int to_st, char s);
	
	// Returns start state id.
	int get_start_state() const;
	// Returns terminal state ids.
	std::vector <int> get_terminal_states() const;
	// Returns true if the state with id 'st' is terminal.
	bool is_terminal(int st) const;
	// Returns the size of the current automaton;
	size_t size() const;
	
	// Returns true if the current automaton accepts the given string.
	bool accepts(const std::string &s);
	
	// Constructs an equivalent deterministic automaton, which contains
	// as minimal states as possible.
	DeterministicAutomaton minimize() const;

	// Returns the automaton intersection of automatons a and b.
	// Both a and b should be complete. Otherwise intersect may throw an
	// invalid argument exception.
	friend DeterministicAutomaton intersect(
		const DeterministicAutomaton &a, const DeterministicAutomaton &b);
	
	// Prints the automaton.
	void print() const;

  private:
	struct Vertex {
		// go stores automaton edges from current vertex.
		std::map <char, int> go;
		
		bool is_terminal;
		
		Vertex(): is_terminal(false) {}
	};
	
	std::vector <Vertex> _automaton;
	int _start;
	
	// Deletes all states that can not be reached from the start state.
	void _delete_unreachable_states();
	
	// Adds new sink state and all edges leading to it to make the
	// automaton complete. If the automaton is already complete, then
	// no new state will be added.
	void _complete();

	// Deletes the st state. Numeration of other vertexes may change.
	void _delete_state(int st);
};

// DeterministicAutomaton stores a non-deterministic finite state machines
// (can have multiple edges from one vertex with the same symbol and
// can have eps-edges).
class NonDeterministicAutomaton {
  public:
	// Creates an automaton with one start state and no terminal states.
	NonDeterministicAutomaton();
	
	// Creates a new state and returns its identificator.
	int add_state();
	
	// Mark and unmark a state with given id terminal.
	void mark_terminal(int st);
	void unmark_terminal(int st);

	// Adds an edge with symbol s between states with id from_st and to_st.
	// '\0' char is reserved for eps-edes.
	void add_edge(int from_st, int to_st, char s);
	// Adds an epsilon edge between states with id from_st and to_st.
	void add_eps_edge(int from_st, int to_st);
	
	// Copies automaton a to the current automaton from state st
	// (copies terminal states from a as well).
	void concatenate_with(const NonDeterministicAutomaton &a, int st);
	
	// Returns start state id.
	int get_start_state() const;
	// Returns terminal state ids.
	std::vector <int> get_terminal_states() const;
	// Returns true if the state with id 'st' is terminal.
	bool is_terminal(int st) const;
	// Returns the size of the current automaton;
	size_t size() const;
	
	// Returns true if the current automaton accepts the given string.
	bool accepts(const std::string &s);
	
	// Returns a deterministic automaton corresponding to the current one.
	DeterministicAutomaton make_deterministic() const;

	// Returns the automaton intersection of automatons a and b.
	// Both a and b should be complete. Otherwise intersect may throw an
	// invalid argument exception.
	friend DeterministicAutomaton intersect(
		const NonDeterministicAutomaton &a, const NonDeterministicAutomaton &b);

  private:
	struct Vertex {
		// go stores automaton edges from current vertex.
		std::map <char, std::vector <int> > go;
		
		bool is_terminal;
		
		Vertex(): is_terminal(false) {}
	};
	
	std::vector <Vertex> _automaton;
	int _start;
	
	// _step_once adds in res all vertexes which can be reached in one symbol from
	// vertexes in st (eps-edges are counted as 0 symbols) and a symbol used
	// for reaching them.
	void _step_once(const std::vector <int> &st, std::vector <std::pair <char, int> > &res) const;
	// _step_once adds in res all vertexes which can be reached in only eps-edges.
	void _step_eps(const std::vector <int> &st, std::vector <int> &res) const;
	
	bool _accepts_from_vertex(int st, const std::string &s, int i = 0);
	
	static const char EPS_EDGE_SYMBOL = '\0';
};

#endif
