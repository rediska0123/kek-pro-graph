#include "parseRDF.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	char *filename = argv[1];
	vector <Edge> edges = parseRDF(filename);
	
	for (Edge e: edges) {
		cout << e.from_label << " " << e.edge_label << " " << e.to_label << endl;
	}
	
	return 0;
}
