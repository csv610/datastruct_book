template <typename T, template <typename...> class Container = std::vector>
class MyCollection {
    Container<T> items_;
};
