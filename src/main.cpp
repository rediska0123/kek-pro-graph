#include "regex_to_fsa.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	NonDeterministicAutomaton a;
	int s = a.get_start_state();
	int x = a.add_state();
	a.add_edge(s, x, 'b');
	a.add_edge(x, x, 'c');
	a.add_edge(x, x, 'd');
	a.mark_terminal(x);
	// b(c|d)*
	
	DeterministicAutomaton d = a.make_deterministic();
	
	cout << a.accepts("bcdc") << endl;
	cout << a.accepts("bcdc") << endl;
	
	return 0;
}
