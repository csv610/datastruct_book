template <typename T>
struct node {
    T data;
    std::unique_ptr<node> next;

    explicit node(const T& value) : data(value), next(nullptr) {}
    explicit node(T&& value) : data(std::move(value)), next(nullptr) {}
};
