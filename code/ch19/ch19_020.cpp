#include <vector>
#include <functional>
#include <cmath>

class LinearCounter {
public:
    explicit LinearCounter(size_t m)
        : m_(m), bits_(m, false), count_(0) {}

    void add(uint64_t hash) {
        size_t idx = hash % m_;
        if (!bits_[idx]) {
            bits_[idx] = true;
            ++count_;
        }
    }

    double estimate() const {
        if (count_ == m_) return static_cast<double>(m_);
        double V = static_cast<double>(m_ - count_);
        return -m_ * std::log(V / m_);
    }

    double load_factor() const {
        return static_cast<double>(count_) / m_;
    }

    size_t memory_bytes() const {
        return m_ / 8 + 2 * sizeof(size_t);
    }

private:
    size_t m_;
    std::vector<bool> bits_;
    size_t count_;
};
