// WRONG -- compiler may optimize the entire loop away
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
