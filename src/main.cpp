#include "regex_to_fsa.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	string regex = "(a|b)*c";
	
	DeterministicAutomaton a = regex_to_fsa(regex);
	
	cout << a.accepts("ac") << endl;
	cout << a.accepts("bc") << endl;
	cout << a.accepts("c") << endl;
	cout << a.accepts("abaac") << endl;
	cout << a.accepts("e") << endl;
	
	return 0;
}
