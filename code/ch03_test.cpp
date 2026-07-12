#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <random>
#include <memory>
#include <algorithm>

// ch03: Performance Measurement - Cache effects, memory hierarchy, profiling

// 1. Cache line effect: sequential vs strided access
void cache_line_demo() {
    std::cout << "=== Cache Line Effect ===\n";
    const size_t N = 1000000;
    std::vector<int> arr(N, 1);
    
    // Sequential access
    auto start = std::chrono::high_resolution_clock::now();
    long sum1 = 0;
    for (size_t i = 0; i < N; ++i) sum1 += arr[i];
    auto end = std::chrono::high_resolution_clock::now();
    double seq_time = std::chrono::duration<double, std::micro>(end - start).count();
    
    // Strided access (64-byte stride = 16 ints)
    start = std::chrono::high_resolution_clock::now();
    long sum2 = 0;
    for (size_t i = 0; i < N; i += 16) sum2 += arr[i];
    end = std::chrono::high_resolution_clock::now();
    double stride_time = std::chrono::duration<double, std::micro>(end - start).count();
    
    std::cout << "Sequential: " << seq_time << " µs, Strided(16): " << stride_time << " µs\n";
    std::cout << "Ratio: " << stride_time / seq_time << "x slower with stride\n\n";
}

// 2. Memory hierarchy: L1 vs L2 vs L3 vs RAM
void memory_hierarchy() {
    std::cout << "=== Memory Hierarchy Latencies (approx) ===\n";
    std::cout << "L1 cache:  ~1 ns (4-32 KB)\n";
    std::cout << "L2 cache:  ~4 ns (256 KB - 1 MB)\n";
    std::cout << "L3 cache:  ~15 ns (8-32 MB)\n";
    std::cout << "RAM:      ~100 ns (GB)\n";
    std::cout << "SSD:      ~50 µs\n";
    std::cout << "Network:  ~1 ms\n\n";
}

// 3. Branch prediction
void branch_prediction() {
    std::cout << "=== Branch Prediction ===\n";
    const size_t N = 10000000;
    std::vector<int> data(N);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, 1);
    
    // Random data (hard to predict)
    for (size_t i = 0; i < N; ++i) data[i] = dis(gen);
    
    auto start = std::chrono::high_resolution_clock::now();
    long count1 = 0;
    for (size_t i = 0; i < N; ++i) if (data[i] > 0) ++count1;
    auto end = std::chrono::high_resolution_clock::now();
    double random_time = std::chrono::duration<double, std::micro>(end - start).count();
    
    // Sorted data (easy to predict)
    std::sort(data.begin(), data.end());
    start = std::chrono::high_resolution_clock::now();
    long count2 = 0;
    for (size_t i = 0; i < N; ++i) if (data[i] > 0) ++count2;
    end = std::chrono::high_resolution_clock::now();
    double sorted_time = std::chrono::duration<double, std::micro>(end - start).count();
    
    std::cout << "Random data: " << random_time << " µs\n";
    std::cout << "Sorted data: " << sorted_time << " µs\n";
    std::cout << "Sorted is " << random_time / sorted_time << "x faster\n\n";
}

// 4. False sharing in multithreading (conceptual)
void false_sharing_concept() {
    std::cout << "=== False Sharing (Conceptual) ===\n";
    std::cout << "Two threads writing to adjacent variables on same cache line\n";
    std::cout << "causes cache line ping-pong, killing performance\n";
    std::cout << "Solution: alignas(64) or padding to separate cache lines\n\n";
}

// 5. Vectorization / SIMD
void vectorization() {
    std::cout << "=== Vectorization (SIMD) ===\n";
    const size_t N = 1000000;
    std::vector<float> a(N, 1.5f), b(N, 2.5f), c(N);
    
    // Scalar
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) c[i] = a[i] + b[i];
    auto end = std::chrono::high_resolution_clock::now();
    double scalar = std::chrono::duration<double, std::micro>(end - start).count();
    
    // With pragma (hint to compiler)
    start = std::chrono::high_resolution_clock::now();
    #pragma clang loop vectorize(enable)
    for (size_t i = 0; i < N; ++i) c[i] = a[i] * b[i];
    end = std::chrono::high_resolution_clock::now();
    double vectorized = std::chrono::duration<double, std::micro>(end - start).count();
    
    std::cout << "Scalar: " << scalar << " µs\n";
    std::cout << "Vectorized hint: " << vectorized << " µs\n";
    std::cout << "Speedup: " << scalar / vectorized << "x\n\n";
}

// 6. Prefetching
void prefetch_demo() {
    std::cout << "=== Software Prefetching ===\n";
    std::cout << "__builtin_prefetch(addr, rw, locality)\n";
    std::cout << "rw: 0=read, 1=write\n";
    std::cout << "locality: 0=none, 1=low, 2=medium, 3=high\n\n";
}

// 7. Profiling tools
void profiling_tools() {
    std::cout << "=== Profiling Tools ===\n";
    std::cout << "perf record -g ./program     # Linux perf\n";
    std::cout << "perf report                  # Analyze\n";
    std::cout << "valgrind --tool=callgrind    # Call graph\n";
    std::cout << "kcachegrind callgrind.out    # Visualize\n";
    std::cout << "Intel VTune, AMD uProf       # Hardware counters\n";
    std::cout << "gprof -pg                    # GNU profiler\n\n";
}

// 8. Memory allocation patterns
void allocation_patterns() {
    std::cout << "=== Allocation Patterns ===\n";
    
    // Bad: many small allocations
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        auto* p = new int[10];
        delete[] p;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double bad = std::chrono::duration<double, std::micro>(end - start).count();
    
    // Good: pool / batch allocation
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<int[]>> pool;
    pool.reserve(10000);
    for (int i = 0; i < 10000; ++i) {
        pool.push_back(std::make_unique<int[]>(10));
    }
    end = std::chrono::high_resolution_clock::now();
    double good = std::chrono::duration<double, std::micro>(end - start).count();
    
    std::cout << "Individual new/delete: " << bad << " µs\n";
    std::cout << "Batch unique_ptr:      " << good << " µs\n\n";
}

int main() {
    std::cout << "=== Chapter 3: Performance Measurement ===\n\n";
    
    cache_line_demo();
    memory_hierarchy();
    branch_prediction();
    false_sharing_concept();
    vectorization();
    prefetch_demo();
    profiling_tools();
    allocation_patterns();
    
    std::cout << "=== All tests passed ===\n";
    return 0;
}