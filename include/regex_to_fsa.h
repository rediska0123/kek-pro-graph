#ifndef REGEX_TO_FSA
#define REGEX_TO_FSA

#include "automaton.h"
#include <string>

// regex_to_fsa returns an deterministic finite state machine
// corresponding to the given regular expression.
//
// Regex supported operations:
//  A|B					union of two regular expressions A and B
//  A*					star (repeat any number of times)
//  (A)					separates A (as usual brackets in arithmetic expressions)
//  \(, \), \*, \|		simple (, ), *, | symbols
// Priorities: | < concatention (no symbol) < *
//   priorities examlpes:  a|ab*|abc ~ ((a)|(a(b*))|((ab)c))
// \ symbol ('\\') can only be used before one of the '(', ')', '*', '|'
//
// Examples:  abc matches        (a|b|c)*c
//            abb does not match (a|b|c)*c
DeterministicAutomaton regex_to_dfsa(const std::string &regex);

#endif
