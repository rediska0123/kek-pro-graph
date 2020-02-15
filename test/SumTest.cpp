#include "SumTest.h"
#include "sum.h"

void SumTest::test_positive() {
	DO_CHECK(sum(2, 3) == 5);
}

void SumTest::test_negative() {
	DO_CHECK(sum(-1, -4) == -5);
}

void SumTest::run_all_tests() {
	test_positive();
	test_negative();
}

