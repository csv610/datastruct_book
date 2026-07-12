std::unordered_map<int, int> sparse;
sparse[1000] = 42;
sparse[5000] = 7;

int val = sparse.count(1000) ? sparse[1000] : 0;
