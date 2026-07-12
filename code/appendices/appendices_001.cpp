using clock = std::chrono::steady_clock;
auto start = clock::now();
// ... work ...
auto end = clock::now();
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
