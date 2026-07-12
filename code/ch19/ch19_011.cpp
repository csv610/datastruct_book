class simhash {
public:
    simhash(size_t bits, size_t dim)
        : bits_(bits), planes_(bits, std::vector<double>(dim)) {
        std::mt19937 rng(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);
        for (auto& v : planes_)
            for (auto& x : v) x = dist(rng);
    }

    uint64_t signature(std::span<const double> vec) const {
        uint64_t sig = 0;
        for (size_t i = 0; i < bits_; ++i) {
            double dot = 0.0;
            for (size_t j = 0; j < vec.size(); ++j)
                dot += planes_[i][j] * vec[j];
            if (dot > 0) sig |= (1ULL << i);
        }
        return sig;
    }

    // Hamming distance on signatures approximates cosine distance
    static double cosine_similarity(uint64_t a, uint64_t b) {
        return 1.0 - static_cast<double>(std::popcount(a ^ b)) / 64.0;
    }

private:
    size_t bits_;
    std::vector<std::vector<double>> planes_;
};
