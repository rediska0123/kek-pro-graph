#include "parseRDF.h"

using namespace std;

int main(int argc, char *argv[]) {
	char *filename = argv[1];
	parseRDF(filename);
	
	return 0;
}
