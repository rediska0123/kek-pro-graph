#include "regex_to_fsa.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	DeterministicAutomaton d = regex_to_dfsa(string(argv[1]));
	
	d.print();
	
	return 0;
}
