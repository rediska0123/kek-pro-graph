#ifndef AUTOMATON_TEST_H
#define AUTOMATON_TEST_H

#include "Test.h"

class AutomatonTest: public Test {
private:
	void test_nondeterministic_construction_and_accepts();
	void test_nondeterministic_concatenate();
	void test_make_deterministic();

	void test_deterministic_construction_and_accepts();
	void test_deterministic_minimize();

	void test_intersection();

public:
	void run_all_tests();
};

#endif
