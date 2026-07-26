#include "doubly_linked_list.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    dsa::doubly_linked_list<int> list;
    assert(list.empty());
    assert(list.size() == 0);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_front(0);
    assert(list.size() == 4);
    assert(list.front() == 0);
    assert(list.back() == 3);

    // Iterator
    std::string s;
    for (auto it = list.begin(); it != list.end(); ++it) {
        s += std::to_string(*it);
    }
    assert(s == "0123");

    list.pop_front();
    assert(list.front() == 1);
    list.pop_back();
    assert(list.back() == 2);
    assert(list.size() == 2);

    list.push_back(3);
    list.push_back(4);
    list.push_back(3);
    list.remove(3);
    assert(list.size() == 3);

    s.clear();
    for (auto it = list.begin(); it != list.end(); ++it)
        s += std::to_string(*it);
    assert(s == "124");

    list.clear();
    assert(list.empty());

    std::print("All doubly_linked_list tests passed\n");
    return 0;
}
