#ifndef TEST_H
#define TEST_H

#include <cstddef>

class Test {
protected:
	static int failed_num;
	static int total_num;
	
public:	
	static void check(bool expr, const char *func, const char *filename, std::size_t lineNum);
	static int show_final_result();
	
	virtual void run_all_tests() = 0;
};

#define DO_CHECK(EXPR) check(EXPR, __FUNCTION__, __FILE__, __LINE__)

#endif

