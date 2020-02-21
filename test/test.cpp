#include "ParseRDFTest.h"
#include <cassert>

int main() {
    ParseRDFTest st;
    st.run_all_tests();
    
    assert(ParseRDFTest::show_final_result() == 0);
    
    return 0;
}
