persistent_list<int> empty;
auto v1 = empty.prepend(3).prepend(2).prepend(1);
auto v2 = v1.prepend(0);

// v1: 1 -> 2 -> 3 -> empty
// v2: 0 -> 1 -> 2 -> 3 -> empty
// v1 is still valid; nodes 1 -> 2 -> 3 -> empty are shared.
