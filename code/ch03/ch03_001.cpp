#include <chrono>
#include <iostream>

class timer {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    time_point start_;
public:
    timer() : start_(clock::now()) {}
    void reset() { start_ = clock::now(); }
    double elapsed() const {
        auto end = clock::now();
        return std::chrono::duration<double>(end - start_).count();
    }
};

// Usage:
timer t;
// code to measure
do_something();
std::cout << "Elapsed: " << t.elapsed() << " s\n";
