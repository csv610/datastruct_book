// Pass by value -- a copy is made
int square(int x) {
    return x * x;
}

// Pass by reference -- no copy, can modify
void increment(int& x) {
    ++x;
}

// Pass by const reference -- no copy, read-only
int sum(const std::vector<int>& values) {
    int s = 0;
    for (int v : values) s += v;
    return s;
}
