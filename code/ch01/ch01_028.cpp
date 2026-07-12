#include <concepts>
#include <memory>
#include <cstddef>

template <typename T>
class simple_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    simple_vector() = default;
    
    explicit simple_vector(size_type n)
        : size_(n), capacity_(n), data_(std::make_unique<T[]>(n)) {}
    
    // Rule of Zero -- unique_ptr handles everything
    
    T& operator[](size_type i) { return data_[i]; }
    const T& operator[](size_type i) const { return data_[i]; }
    
    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    
private:
    size_type size_ = 0;
    size_type capacity_ = 0;
    std::unique_ptr<T[]> data_;
};
