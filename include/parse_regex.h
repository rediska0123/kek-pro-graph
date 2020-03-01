#ifndef PARSE_REGEX_H
#define PARSE_REGEX_H

#include "automation.h"
#include <vector>
#include <string>

enum node_type {
	SYMBOL,		// a, b, c, ...
	UNION, 		// |
	STAR,		// *
	CONCAT		// no symbol
};

struct TreeNode {
	char x;
	
	node_type type;
	
	// 1  edge if x == '*'
	// 2 edges if x == '|'
	// 0 edges if x is other symbol
	std::vector <TreeNode*> edges;
	
	TreeNode(char x, node_type type, std::vector <TreeNode*> edges):\
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
// Priorities: | < concatention (no symbol) < * < ()
// '\' symbol can only be used before one of the '(', ')', '*', '|'
//
// Example:  abc matches (a|b|c)*c
TreeNode *parse_regex(std::string regex);

#endif
