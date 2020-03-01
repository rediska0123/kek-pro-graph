#include "parse_regex.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void printTree(TreeNode *x, int deep = 0) {
	for (int i = 0; i < deep; i++)
		printf("\t");
	printf("%c\n", x->x);
	for (int i = 0; i < (int)x->edges.size(); i++)
		printTree(x->edges[i], deep + i);
}

int main(int argc, char *argv[]) {
	TreeNode *tn = parse_regex("a|bc|d");
	printTree(tn);
	
	return 0;
}
