[![Build Status](https://travis-ci.com/rediska0123/kek-pro-graph.svg?branch=master)](https://travis-ci.org/rediska0123/kek-pro-graph)

# Kek Pro Graph
какой-то кек про граф

## Installing instructions (Linux OS)

From kek-pro-graph directory:
```
git submodule update --init --recursive
```
and from kek-pro-graph/serd directory:
```
./waf configure
./waf
sudo ./waf install
```

## Building instructions

To build the project simply run
```
make
make tests
```

### Running tests instructions

```
make tests
./tests
```

### Constructing minimal deterministic automaton for a regular expression

To construct a minimal determinstic automaton run:
```
make
./main 'your_regular expression'
```
The program will print the automaton: number of states and each state's description: its edges and whether it is terminal.
The start state is always state number 0.

Output example:
```
$ ./main '(ab|aab|aba)*'
5 states: 
0: terminal
	0 -> 1 : a
1: 
	1 -> 2 : a
	1 -> 3 : b
2: 
	2 -> 0 : b
3: terminal
	3 -> 4 : a
4: terminal
	4 -> 1 : a
	4 -> 3 : b
```

#### Regular expression syntax:

Regex supported operations:
| | |
|-|-|
| A|B                   | union of two regular expressions A and B                  |
| A*                    | star (repeat any number of times)                         |
| (A)                   | separates A (as usual brackets in arithmetic expressions) |
| \(, \), \*, \|        | simple (, ), *, |, \ symbols                              |

###### Priorities
| < concatention (no symbol) < *

  priorities examlpes:  a|ab*|abc ~ ((a)|(a(b*))|((ab)c))

\ symbol can only be used before one of the '(', ')', '*', '|'

Examples:  abc matches        '(a|b|c)*c'
           abb does not match '(a|b|c)*c'
Other correct expressions:

 * '(a|b|c)*kek(a|b|c)*'
 * '\\*' --- '*'
 * '\\**' --- any number of '*' symbols
