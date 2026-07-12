template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
T sum(T a, T b) { return a + b; }
