#include "Test.h"
#include <cstdio>

void Test::check(bool expr, const char *func, const char *file, size_t line_num) {
	if (!expr) {
		printf("test failed: %s() in %s:%zu\n", func, file, line_num);
		failed_num++;
	}
	total_num++;
}

int Test::show_final_result() {
	if (failed_num == 0) {
		printf("All tests passed.\n");
		return 0;
	} else {
		printf("Failed %d of %d tests\n", failed_num, total_num);
		return 1;
	}
}

int Test::total_num = 0;
int Test::failed_num = 0;
