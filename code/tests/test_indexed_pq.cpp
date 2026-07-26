#include "indexed_pq.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

int main() {
    // Indexed min-priority queue
    {
        dsa::indexed_min_pq<int> pq(5);
        pq.push(0, 10);
        pq.push(1, 5);
        pq.push(2, 15);
        pq.push(3, 3);
        pq.push(4, 8);

        assert(pq.top_key() == 3);
        assert(pq.pop() == 3);
        assert(pq.top_key() == 5);
        assert(pq.pop() == 1);
    }

    // Decrease key
    {
        dsa::indexed_min_pq<int> pq(5);
        pq.push(0, 10);
        pq.push(1, 20);
        pq.push(2, 15);

        pq.decrease_key(1, 5);
        assert(pq.top_key() == 5);
        assert(pq.pop() == 1);
    }

    // Contains check
    {
        dsa::indexed_min_pq<int> pq(5);
        pq.push(0, 10);
        assert(pq.contains(0));
        assert(!pq.contains(3));
        pq.pop();
        assert(!pq.contains(0));
    }

    // Indexed max-priority queue
    {
        dsa::indexed_max_pq<int> pq(5);
        pq.push(0, 10);
        pq.push(1, 5);
        pq.push(2, 15);
        pq.push(3, 3);

        assert(pq.top_key() == 15);
        assert(pq.pop() == 2);
        assert(pq.top_key() == 10);
    }

    // Increase key
    {
        dsa::indexed_max_pq<int> pq(5);
        pq.push(0, 10);
        pq.push(1, 5);
        pq.increase_key(1, 20);
        assert(pq.top_key() == 20);
        assert(pq.pop() == 1);
    }

    std::println("All indexed PQ tests passed");
    return 0;
}
