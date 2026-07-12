template <typename T>
struct plist_node {
    T head;
    std::shared_ptr<const plist_node<T>> tail;
};

template <typename T>
class persistent_list {
public:
    persistent_list() : root_(nullptr) {}

    persistent_list(const T& val,
                    const persistent_list& rest)
        : root_(std::make_shared<plist_node<T>>(
            plist_node<T>{val, rest.root_})) {}

    bool empty() const { return !root_; }

    const T& head() const { return root_->head; }

    persistent_list tail() const {
        return persistent_list(root_->tail);
    }

    persistent_list prepend(const T& val) const {
        return persistent_list(val, *this);
    }

    size_t size() const {
        size_t n = 0;
        auto cur = root_;
        while (cur) { ++n; cur = cur->tail; }
        return n;
    }

private:
    std::shared_ptr<const plist_node<T>> root_;

    explicit persistent_list(
        std::shared_ptr<const plist_node<T>> node)
        : root_(node) {}
};
