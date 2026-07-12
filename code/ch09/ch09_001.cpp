template <typename T>
struct binary_node {
    T data;
    std::unique_ptr<binary_node> left;
    std::unique_ptr<binary_node> right;

    explicit binary_node(const T& value)
        : data(value), left(nullptr), right(nullptr) {}
    explicit binary_node(T&& value)
        : data(std::move(value)), left(nullptr), right(nullptr) {}
};
