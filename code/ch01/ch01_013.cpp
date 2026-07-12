template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template <Comparable T>
T min_value(T a, T b) {
    return a < b ? a : b;
}

// Equivalent abbreviated form:
auto min_value(Comparable auto a, Comparable auto b) {
    return a < b ? a : b;
}
