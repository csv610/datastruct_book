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
