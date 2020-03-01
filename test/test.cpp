#include "ParseRDFTest.h"
#include "parse_regex_test.h"
#include <cassert>

int main() {
    ParseRDFTest st;
    st.run_all_tests();
    
    assert(ParseRDFTest::show_final_result() == 0);
    
    ParseRegexTest rt;
    rt.run_all_tests();
    
    assert(ParseRegexTest::show_final_result() == 0);
    
    return 0;
}
