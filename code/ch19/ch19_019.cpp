#include <vector>
#include <random>
#include <iterator>

template <typename Iter>
std::vector<typename std::iterator_traits<Iter>::value_type>
reservoir_sample(Iter begin, Iter end, size_t k,
                 std::mt19937& rng)
{
    std::vector<typename std::iterator_traits<Iter>::value_type>
        R;
    R.reserve(k);

    size_t i = 0;
    for (auto it = begin; it != end; ++it, ++i) {
        if (i < k) {
            R.push_back(*it);
        } else {
            std::uniform_int_distribution<size_t>
                dist(0, i);
            size_t j = dist(rng);
            if (j < k)
                R[j] = *it;
        }
    }
    return R;
}

template <typename T>
class StreamSampler {
public:
    explicit StreamSampler(size_t k, uint64_t seed = 42)
        : k_(k), rng_(seed), n_(0) {
        R_.reserve(k);
    }

    void add(const T& value) {
        if (n_ < k_) {
            R_.push_back(value);
        } else {
            std::uniform_int_distribution<size_t>
                dist(0, n_);
            size_t j = dist(rng_);
            if (j < k_)
                R_[j] = value;
        }
        ++n_;
    }

    const std::vector<T>& sample() const { return R_; }
    size_t total_count() const { return n_; }

private:
    size_t k_;
    std::mt19937 rng_;
    uint64_t n_;
    std::vector<T> R_;
};
