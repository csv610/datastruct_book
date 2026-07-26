#include "skip_list.h"
#include <cassert>
#include <string>
#include <vector>
#include <print>

int main() {
    dsa::skip_list<int> sl;
    sl.insert(3);
    sl.insert(1);
    sl.insert(4);
    sl.insert(1);
    sl.insert(5);
    sl.insert(9);
    sl.insert(2);

    assert(sl.size() == 6);
    assert(sl.contains(1));
    assert(sl.contains(5));
    assert(!sl.contains(10));

    auto sorted = sl.to_sorted_vector();
    std::vector<int> expected = {1, 2, 3, 4, 5, 9};
    assert(sorted == expected);

    assert(sl.erase(4));
    assert(!sl.contains(4));
    assert(sl.size() == 5);

    assert(!sl.erase(100));

    // String skip list
    dsa::skip_list<std::string> ssl;
    ssl.insert("cherry");
    ssl.insert("apple");
    ssl.insert("banana");
    assert(ssl.size() == 3);
    auto s = ssl.to_sorted_vector();
    assert(s[0] == "apple" && s[1] == "banana" && s[2] == "cherry");

    std::print("All skip list tests passed\n");
    return 0;
}
