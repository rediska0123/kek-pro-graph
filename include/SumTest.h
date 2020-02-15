#pragma once

#include "Test.h"

class SumTest: public Test {
private:
	void test_positive();
	void test_negative();
	
public:
	void run_all_tests();
};
