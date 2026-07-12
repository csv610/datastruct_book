#include <array>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <bit>

class HyperLogLog {
public:
    explicit HyperLogLog(uint8_t p)
        : p_(p), m_(1u << p), alpha_(0.7213 / (1.0 + 1.079 / m_))
    {
        M_.fill(0);
    }

    void add(uint64_t hash) {
        uint32_t idx = hash >> (64 - p_);
        uint64_t w = hash << p_ | (1ULL << (p_ - 1));
        uint8_t rho = static_cast<uint8_t>(
            std::countl_zero(w) + 1);
        M_[idx] = std::max(M_[idx], rho);
    }

    double estimate() const {
        double sum = 0.0;
        int zeros = 0;
        for (int j = 0; j < m_; ++j) {
            sum += std::pow(2.0, -M_[j]);
            if (M_[j] == 0) ++zeros;
        }
        double E = alpha_ * m_ * m_ / sum;
        if (E <= 2.5 * m_ && zeros > 0) {
            E = m_ * std::log(static_cast<double>(m_) / zeros);
        }
        if (E > (1ULL << 32) / 30.0) {
            E = -std::pow(2.0, 32) *
                std::log1p(-E / std::pow(2.0, 32));
        }
        return E;
    }

    void merge(const HyperLogLog& other) {
        for (int j = 0; j < m_; ++j)
            M_[j] = std::max(M_[j], other.M_[j]);
    }

    size_t memory_bytes() const {
        return m_ + 2 * sizeof(uint8_t) + sizeof(int);
    }

private:
    uint8_t p_;
    int m_;
    double alpha_;
    std::array<uint8_t, 1 << 16> M_;
};
