template <typename T>
struct dnode {
    T data;
    std::unique_ptr<dnode> next;
    dnode* prev;

    explicit dnode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};
