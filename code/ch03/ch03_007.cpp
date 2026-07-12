static void BM_Good(benchmark::State& state) {
    int sum = 0;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            sum += i;
        }
    }
    benchmark::DoNotOptimize(sum);
}
