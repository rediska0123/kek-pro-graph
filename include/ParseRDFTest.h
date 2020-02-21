#ifndef PARSE_RDF_TEST_H
#define PARSE_RDF_TEST_H

#include "Test.h"

class ParseRDFTest: public Test {
private:
	void test_empty_file();
	void test_single_edge();
	void test_general();
	
public:
	void run_all_tests();
};

#endif
