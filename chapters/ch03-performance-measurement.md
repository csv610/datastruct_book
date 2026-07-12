# Performance Measurement of Programs

## Introduction

In Chapter 2 we analyzed algorithms theoretically. But theory is not enough. Constants matter. Cache behavior matters. Compiler optimizations matter. The only way to know how fast a program actually runs is to measure it.

This chapter covers practical techniques for measuring program performance using modern tools.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Why Measure?

- **Validate theoretical analysis** — does the O(n²) algorithm actually scale quadratically?
- **Choose between algorithms** — quicksort is O(n log n) on average, but for small arrays insertion sort is faster
- **Find bottlenecks** — the 90/10 rule: 90% of time is spent in 10% of the code
- **Track regressions** — did a change slow things down?
- **Tune performance** — iterative improvement requires measurement

## A First Measurement: std::chrono

C++11 introduced the `<chrono>` library for portable high-resolution timing:

```cpp
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
```

### Clock Types

| Clock | Characteristics |
|-------|----------------|
| `system_clock` | System-wide real-time clock, can be adjusted |
| `steady_clock` | Monotonic — never adjusted, ideal for intervals |
| `high_resolution_clock` | Highest precision available (often an alias for steady_clock) |

**Always use `steady_clock` for performance measurement** — it is immune to system time adjustments.

### Measuring Short Intervals

For very short operations, the measurement overhead can dominate. Always measure multiple repetitions:

```cpp
template <typename F>
double measure(F&& f, int iterations = 1000) {
    timer t;
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    return t.elapsed() / iterations;
}
```

## Google Benchmark Library

For serious benchmarking, use Google Benchmark. It handles warm-up, iteration count, statistical analysis, and output formatting:

```cpp
#include <benchmark/benchmark.h>

static void BM_VectorPushBack(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_VectorPushBack)->Range(8, 8<<10);

BENCHMARK_MAIN();
```

Compile and run:

```bash
g++ -std=c++20 -O2 -lbenchmark benchmark.cpp -o benchmark
./benchmark
```

Output:

```
BM_VectorPushBack/8       25.3 ns  25.1 ns   27692222
BM_VectorPushBack/64       151 ns   148 ns    4716981
BM_VectorPushBack/512     1186 ns  1175 ns     597163
BM_VectorPushBack/4096   10235 ns 10123 ns      68790
BM_VectorPushBack/8192   41234 ns 40987 ns      17021
```

## Factors Affecting Performance

### Hardware Effects

- **CPU speed and architecture** — clock rate, superscalar execution, vector instructions
- **Cache hierarchy** — L1 (~32KB, ~1ns), L2 (~256KB, ~4ns), L3 (~8MB, ~10ns), RAM (~100ns)
- **Memory bandwidth** — sustained throughput to main memory
- **Branch prediction** — mispredictions cost 10-20 cycles
- **TLB** — virtual address translation cache

### Operating System Effects

- **Context switching** — other processes interrupt your program
- **Page faults** — accessing memory not in physical RAM
- **CPU frequency scaling** — turbo boost, power saving modes

**Mitigation:** Run benchmarks multiple times, use warm-up phases, run on an idle system.

### Compiler Effects

- **Optimization level** (`-O0`, `-O1`, `-O2`, `-O3`, `-Ofast`)
- **Inlining** — functions may be inlined or not
- **Loop unrolling** — trade code size for speed
- **Vectorization** — auto-vectorization of SIMD instructions
- **Dead code elimination** — the compiler may remove unused computations

**Important:** Always benchmark with optimizations enabled (`-O2` or `-O3`). Debug builds (`-O0`) give meaningless timing.

### The Benchmarking Trap: Dead Code Elimination

The compiler may optimize away the computation you are trying to measure:

```cpp
// WRONG — compiler may optimize the entire loop away
benchmark::DoNotOptimize(0);  // use this marker instead
static void BM_Bad(benchmark::State& state) {
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            sum += i;  // compiler may notice 'sum' is unused
        }
    }
}
// The compiler can eliminate the entire inner loop since 'sum' has no observable effect
```

Always consume the result:

```cpp
static void BM_Good(benchmark::State& state) {
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            sum += i;
        }
    }
    benchmark::DoNotOptimize(sum);
}
```

## Case Study: Sorting Algorithms

Let us measure insertion sort, merge sort, and std::sort:

```cpp
void insertion_sort(std::span<int> a) {
    for (size_t i = 1; i < a.size(); ++i) {
        int key = a[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

void merge_sort(std::span<int> a) {
    if (a.size() <= 1) return;
    size_t mid = a.size() / 2;
    std::vector<int> left(a.begin(), a.begin() + mid);
    std::vector<int> right(a.begin() + mid, a.end());
    merge_sort(left);
    merge_sort(right);
    std::merge(left.begin(), left.end(), right.begin(), right.end(), a.begin());
}
```

```cpp
static void BM_InsertionSort(benchmark::State& state) {
    std::vector<int> v(state.range(0));
    for (auto _ : state) {
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        insertion_sort(v);
        benchmark::DoNotOptimize(v.data());
    }
}
BENCHMARK(BM_InsertionSort)->Range(8, 8<<10);

static void BM_MergeSort(benchmark::State& state) {
    std::vector<int> v(state.range(0));
    for (auto _ : state) {
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        merge_sort(v);
        benchmark::DoNotOptimize(v.data());
    }
}
BENCHMARK(BM_MergeSort)->Range(8, 8<<10);

static void BM_StdSort(benchmark::State& state) {
    std::vector<int> v(state.range(0));
    for (auto _ : state) {
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        std::sort(v.begin(), v.end());
        benchmark::DoNotOptimize(v.data());
    }
}
BENCHMARK(BM_StdSort)->Range(8, 8<<10);
```

Expected results:
- Insertion sort: O(n²) — noticeable quadratic curve
- Merge sort: O(n log n) — smoother curve
- `std::sort`: fastest — uses introsort (quicksort + heapsort + insertion sort hybrid)

## Profiling

While benchmarking measures total time, **profiling** shows where time is spent.

### Sampling Profilers

Sample the program counter at regular intervals:

```bash
# macOS
sudo sample ./program -duration 10  # produces a call tree

# Linux
perf record ./program && perf report

# macOS Instruments (GUI)
xcrun xctrace record --template "Time Profiler" --launch ./program
```

### Instrumenting Profilers

**Valgrind/Callgrind:**

```bash
valgrind --tool=callgrind ./program
# Produces callgrind.out.xxx — view with kcachegrind
```

**Compiler instrumentation (`-finstrument-functions`):**

GCC/Clang can insert callbacks at every function entry/exit for custom profiling.

### Cache Analysis with perf

```bash
perf stat -e cache-references,cache-misses,cycles,instructions ./program
```

Sample output:
```
   1,234,567,890      cycles
     987,654,321      instructions
      12,345,678      cache-references
       1,234,567      cache-misses    # 10.0% miss rate
```

A high cache miss rate (>10%) indicates poor data locality.

## Measuring Memory Usage

```cpp
#include <sys/resource.h>

size_t peak_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // in kilobytes (macOS) or bytes (Linux)
}
```

For detailed heap profiling, use **Valgrind/Massif**:

```bash
valgrind --tool=massif ./program
ms_print massif.out.xxx
```

## Practical Measurement Methodology

1. **Formulate a hypothesis** — e.g., "std::sort is faster than my quicksort"
2. **Design the experiment** — what inputs, what sizes, what metrics?
3. **Warm up** — run the code once to populate caches
4. **Measure multiple trials** — at least 10, report mean and standard deviation
5. **Vary input size** — confirm asymptotic class
6. **Vary input type** — random, sorted, reversed, nearly sorted
7. **Isolate the system** — close other applications, disable CPU scaling
8. **Report everything** — compiler, flags, CPU, OS, input parameters

## Common Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Debug build | Measurements include assertions | Use `-O2` |
| No warm-up | Cold cache skews results | Run iterations before measuring |
| Too few iterations | High variance | Use enough iterations for stable mean |
| Dead code elimination | Zero time reported | Consume results with `DoNotOptimize` |
| System load | Inconsistent timing | Run on idle system, report median |
| Clock resolution | Zero elapsed time | Use enough work per measurement |
| Inlining | Function not measured separately | Use `__attribute__((noinline))` |

## Summary

1. **Use `std::chrono::steady_clock`** for portable timing.
2. **Use Google Benchmark** for serious measurement — it handles warm-up, iteration control, and statistics.
3. **Always optimize** (`-O2` or `-O3`) when benchmarking.
4. **Prevent dead code elimination** by consuming results.
5. **Profile** to find bottlenecks; **measure** to verify improvements.
6. **Understand hardware effects** — cache misses dominate for large data sets.
7. **Report methodology** so results are reproducible.

## Exercises

### Drill

1. **Average vs tail latency.** A performance test reports average latency of 2 ms but P99 (99th percentile) of 200 ms. Why are these so different? For a checkout service that must respond within 10 ms, which metric matters? What fraction of customers experience a timeout?

   *(In production, companies like Google and Amazon use P99.9 or P99.99 for SLOs because averages hide the slow requests that hurt user experience.)*

2. **Clock choice.** You measure a function with `std::chrono::steady_clock` and get 4.8 seconds. A colleague uses `std::chrono::system_clock` and gets 5.2 seconds. Both ran the same code on the same machine. What property of `system_clock` (vs `steady_clock`) explains the difference? Which is correct for benchmarking?

3. **Tail latency in servers.** A web server handles 100 concurrent connections. The average response time reported is 2 ms, but P99.9 is 500 ms. An engineer says "the 500 ms is just an outlier — I'll optimize the 2 ms average." Why is this wrong for an e-commerce site? If 0.1% of requests are slow and the site serves 1 million requests/day, how many users are affected?

4. **Dead code elimination.** The following benchmark always reports 0 ns:
   ```cpp
   static void BM_Sort(benchmark::State& state) {
       std::vector<int> v(1000000);
       std::iota(v.begin(), v.end(), 0);
       for (auto _ : state) {
           std::sort(v.begin(), v.end());  // result never used!
       }
   }
   ```
   Why does the compiler optimize away the sort? How do you fix it?

5. **Wall time vs CPU time.** A benchmark shows wall time = 10 s but CPU time = 4 s. Where did the other 6 seconds go? List three possible causes.

   *(In production, burstable cloud instances (like AWS t3) can cause this discrepancy when CPU credits run out.)*

### Application

6. **Throughput-latency measurement.** A message queue ingests data from producers. On a fast server it handles 800 MB/s. On a modest server, design a benchmark that:
   - Measures throughput (MB/s)
   - Reports P50, P99, P99.9 latency
   - Runs for at least 60 seconds to steady state
   - Compares synchronous (wait for confirmation) vs asynchronous (fire-and-forget) modes
   Plot throughput vs latency. What throughput can you guarantee if latency must stay under 50 ms?

7. **Load testing.** A web service gets 1000 req/s at night and 10000 req/s during peak. Load testing at peak shows 12 ms average latency. A colleague says "run at 60% CPU to leave headroom." Another says "90% CPU is fine, latency is 11 ms." Design an experiment that gradually increases load and finds the inflection point where latency starts to spike.

8. **Cache effects.** An analytical query runs 10× slower the first time than subsequent times. The difference is the cache. Design a benchmark that:
   - Warms the cache (run once, discard)
   - Then measures cold cache (drop OS caches)
   - Tests concurrent queries (1, 4, 16 simultaneous)
   - Tests different data sizes (1 GB, 10 GB)
   - Runs each configuration 10+ times for statistical significance
   Report which factor (concurrency, data size, cache state) has the largest impact.

   *(In production, databases like ClickHouse are sensitive to all three — their documentation specifies which knobs to tune.)*

9. **SIMD vs scalar.** Compare an element-wise operation on a large array using plain C++ vs using SIMD intrinsics (SSE/AVX on x86 or NEON on ARM):
   - Element-wise multiplication of 10^7 floats
   - Dot product of two vectors
   Use your platform's timing. Plot time vs array size. At what size does SIMD pull ahead?

10. **False sharing.** Two threads increment adjacent variables in an array. Performance is poor beyond 2 cores. The cause is false sharing: the CPU cache line (typically 64 bytes) holds both variables, so each write invalidates the other thread's copy. Design a benchmark that:
    - Creates an array of atomic counters
    - Launches 2, 4, 8 threads, each incrementing its own counter
    - Measures throughput with and without padding (put each counter in its own cache line)
    - Reports speedup vs thread count

    *(In production, this pattern appears wherever threads update independent but adjacent data — Uber's H3 geospatial indexing library is one example.)*

### Research

11. **Cycle-accurate timing.** Research the `rdtscp` CPU instruction that reads the cycle counter. Write a C++ wrapper. Benchmark `std::chrono::steady_clock::now()` overhead vs your `rdtscp` wrapper over 10^6 calls. Which is more precise? When would a cycle counter give misleading results (hint: CPU frequency scaling)?

12. **Profiling a real system.** Download an open-source database engine (e.g., DuckDB). Build it. Run a standard benchmark query and profile using `perf record` or Instruments. Identify:
    - The hottest 3 functions
    - Percentage of time in hash lookups vs parsing vs I/O
    - Cache miss rates (L1, LLC)
    - Branch misprediction rate
    Propose one optimization based on the profile.

13. **Statistical significance.** Run a sorting benchmark 30 times under identical conditions. Compute mean, std dev, and 95% confidence interval. Then introduce a small change (e.g., switch from `std::sort` to `std::stable_sort`) — can you detect the difference with 30 runs? With 100 runs? How small a change can you reliably detect?

    *(In production, Google's performance lab uses A/A testing (both groups get the same treatment) to measure the noise floor before running actual experiments.)*

14. **Compiler profiling.** Compile a medium-sized C++ project and use your compiler's time-tracing flags (e.g., `-ftime-report` in GCC/Clang) to measure which optimization pass takes the most time. Is it inlining? Loop optimization? Register allocation? Propose one improvement.

## References and Further Reading

- Google Benchmark documentation: https://github.com/google/benchmark
- Chandler Carruth, "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!" — CppCon 2015 talk.
- Agner Fog, "Optimizing Software in C++" — https://www.agner.org/optimize/
- Ulrich Drepper, "What Every Programmer Should Know About Memory" — https://people.freebsd.org/~lstewart/articles/cpumemory.pdf
- John L. Hennessy and David A. Patterson, *Computer Architecture: A Quantitative Approach*, 6th Edition. Morgan Kaufmann, 2017.
- David Levinthal, "Performance Analysis Guide for Intel Core i7 and Intel Xeon 5500 Processors" — Intel, 2009.
- Paul E. McKenney, "Memory Barriers: a Hardware View for Software Hackers" — Linux Technical Conference, 2009.
