template <typename F>
double measure(F&& f, int iterations = 1000) {
    timer t;
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    return t.elapsed() / iterations;
}
