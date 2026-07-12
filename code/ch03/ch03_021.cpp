static void BM_Sort(benchmark::State& state) {
       std::vector<int> v(1000000);
       std::iota(v.begin(), v.end(), 0);
       for (auto _ : state) {
           std::sort(v.begin(), v.end());  // result never used!
       }
   }
