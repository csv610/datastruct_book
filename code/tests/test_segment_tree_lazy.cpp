#include "segment_tree_lazy.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
    // Range sum query with range add
    {
        std::vector<int> data = {1, 3, 5, 7, 9, 11};
        dsa::segment_tree_lazy<int> st(data);

        assert(st.query(0, 5) == 36);       // 1+3+5+7+9+11 = 36
        assert(st.query(1, 3) == 15);       // 3+5+7 = 15
        assert(st.point_query(2) == 5);

        st.update(1, 3, 10);  // add 10 to [1,3] -> {1, 13, 15, 17, 9, 11}
        assert(st.query(0, 5) == 66);
        assert(st.point_query(1) == 13);
        assert(st.point_query(2) == 15);
        assert(st.point_query(3) == 17);
        assert(st.point_query(0) == 1);   // unchanged
        assert(st.point_query(4) == 9);   // unchanged

        // Another update
        st.update(0, 5, 1);  // add 1 to all
        assert(st.point_query(0) == 2);
        assert(st.point_query(4) == 10);
        assert(st.query(0, 5) == 72);  // 66 + 6
    }

    // Single element
    {
        std::vector<int> data = {42};
        dsa::segment_tree_lazy<int> st(data);
        assert(st.query(0, 0) == 42);
        st.update(0, 0, 8);
        assert(st.query(0, 0) == 50);
    }

    // Point update
    {
        std::vector<int> data = {1, 2, 3, 4, 5};
        dsa::segment_tree_lazy<int> st(data);
        st.update(2, 2, 7);  // add 7 to index 2: 3 -> 10
        assert(st.point_query(2) == 10);
        assert(st.query(0, 4) == 22);  // 1+2+10+4+5 = 22
    }

    // Lazy propagation correctness with nested updates
    {
        std::vector<int> data(8, 0);
        dsa::segment_tree_lazy<int> st(data);
        st.update(0, 3, 5);   // [5,5,5,5,0,0,0,0]
        st.update(2, 5, 3);   // [5,5,8,8,3,3,0,0]
        assert(st.query(0, 7) == 32);
        assert(st.query(0, 3) == 26);   // 5+5+8+8
        assert(st.query(4, 7) == 6);    // 3+3+0+0
    }

    std::print("All segment tree lazy tests passed\n");
    return 0;
}
