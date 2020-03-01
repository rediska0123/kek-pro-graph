#include "regex_to_fsa.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	DeterministicAutomaton d = regex_to_dfsa("(ab|aab|aba)*");
	
	cout << d.accepts("aababaabababa") << endl;
	
	d.print();
	
	return 0;
}
