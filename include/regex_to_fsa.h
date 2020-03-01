#ifndef REGEX_TO_FSA
#define REGEX_TO_FSA

#include "automaton.h"
#include <string>

// regex_to_fsa returns an deterministic finite state machine
// corresponding to the given regular expression.
DeterministicAutomaton regex_to_dfsa(const std::string &regex);

#endif
