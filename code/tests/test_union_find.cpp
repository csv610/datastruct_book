#include "union_find.h"
#include <cassert>
#include <print>

int main() {
    dsa::union_find uf(6);
    assert(uf.count() == 6);
    assert(uf.size(0) == 1);

    assert(uf.unite(0, 1));
    assert(uf.connected(0, 1));
    assert(!uf.connected(0, 2));
    assert(uf.count() == 5);
    assert(uf.size(0) == 2);

    assert(uf.unite(2, 3));
    assert(uf.unite(0, 2));
    assert(uf.connected(1, 3));
    assert(uf.size(0) == 4);

    assert(!uf.unite(0, 1));
    assert(uf.unite(4, 5));
    assert(uf.count() == 2);

    assert(uf.unite(0, 4));
    assert(uf.count() == 1);
    assert(uf.size(0) == 6);
    assert(uf.connected(1, 5));

    std::print("All union-find tests passed\n");
    return 0;
}
