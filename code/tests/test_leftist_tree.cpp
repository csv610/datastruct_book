#include "leftist_tree.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    dsa::leftist_tree<int> lt;
    lt.push(5);
    lt.push(3);
    lt.push(8);
    lt.push(1);
    lt.push(4);

    assert(lt.top() == 1);
    lt.pop();
    assert(lt.top() == 3);
    lt.pop();
    assert(lt.top() == 4);

    dsa::leftist_tree<int> lt2;
    lt2.push(2);
    lt2.push(6);
    lt.meld(std::move(lt2));

    std::vector<int> sorted = lt.drain_sorted();
    // lt had {5,3,8,1,4} -> pop 1 -> pop 3 -> top=4 -> lt has {4,5,8}
    // lt2 had {2,6} -> melded -> {2,4,5,6,8}
    assert(sorted.size() == 5);
    assert(sorted[0] == 2);
    assert(sorted[1] == 4);
    assert(sorted[2] == 5);
    assert(sorted[3] == 6);
    assert(sorted[4] == 8);

    // Empty tree
    dsa::leftist_tree<int> empty;
    assert(empty.empty());

    std::cout << "All leftist tree tests passed\n";
    return 0;
}
