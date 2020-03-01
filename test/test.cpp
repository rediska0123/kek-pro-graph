#include "ParseRDFTest.h"
#include "parse_regex_test.h"
#include "automaton_test.h"
#include <cassert>

#define TEST(TestClass)                               \
	{                                                 \
		TestClass tc;                                 \
		tc.run_all_tests();                           \
		assert(TestClass::show_final_result() == 0);  \
	}

int main() {
	TEST(ParseRDFTest);
	TEST(ParseRegexTest);
	TEST(AutomatonTest);
    
    return 0;
}
