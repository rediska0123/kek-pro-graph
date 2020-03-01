#ifndef PARSE_REGEX_H
#define PARSE_REGEX_H

#include <vector>
#include <string>

enum node_type {
	SYMBOL,		// a, b, c, ...
	UNION, 		// |
	STAR,		// *
	CONCAT		// no symbol
};

// TreeNode stores a node of the tree constructed while parsing regular
// expression in parse_regex function below.
struct TreeNode {
	// x is a symbol corresponding to the current node:
	//  '|' if type == UNION,
	//  '*' if type == STAR,
	//  ' ' if type == CONCAT,
	//  symbol from the regular expression, if type == SYMBOL.
	char x;
	
	node_type type;
	
	// 1  edge if type == STAR
	// 2 edges if type == UNION
	// 2 edges if type == CONCAT
	// 0 edges if type == SYMBOL
	std::vector <TreeNode*> edges;
	
	TreeNode(char x, node_type type, std::vector <TreeNode*> edges):
		x(x), type(type), edges(edges) {}
	~TreeNode() {
		for (TreeNode *tn : edges)
			delete tn;
	}
};

// parse_regex makes a parsing tree from a regex.
//
// Regex supported operations:
//  A|B					union of two regular expressions A and B
//  A*					star (repeat any number of times)
//  (A)					separates A (as usual brackets in arithmetic expressions)
//  \(, \), \*, \|		simple (, ), *, |, \ symbols
// Priorities: | < concatention (no symbol) < *
//   priorities examlpes:  a|ab*|abc ~ ((a)|(a(b*))|((ab)c))
// \ symbol ('\\') can only be used before one of the '(', ')', '*', '|'
//
// Examples:  abc matches        (a|b|c)*c
//            abb does not match (a|b|c)*c
TreeNode *parse_regex(const std::string &regex);

#endif
