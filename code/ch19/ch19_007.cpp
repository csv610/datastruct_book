class quantile_estimator {
public:
    explicit quantile_estimator(double epsilon) : epsilon_(epsilon) {}

    void add(double value) {
        summary_.push_back({value, 1, 0});

        // Merge adjacent tuples to maintain size bound
        if (summary_.size() > static_cast<size_t>(1.0 / epsilon_)) {
            merge_tuples();
        }
    }

    double query(double phi) const {
        // Query the phi-quantile (e.g., 0.5 for median)
        if (summary_.empty()) return 0.0;

        double rank = phi * total_count_;
        size_t cumulative = 0;

        for (size_t i = 0; i < summary_.size(); ++i) {
            cumulative += summary_[i].g;
            if (cumulative + summary_[i].delta >= static_cast<size_t>(rank) &&
                cumulative <= static_cast<size_t>(rank)) {
                return summary_[i].value;
            }
        }
        return summary_.back().value;
    }

private:
    struct tuple {
        double value;
        size_t g;      // gap to next
        size_t delta;   // max gap to previous
    };

    void merge_tuples() {
        std::vector<tuple> merged;
        for (size_t i = 0; i + 1 < summary_.size(); ++i) {
            if (summary_[i].g + summary_[i+1].g +
                summary_[i+1].delta <= 2 * epsilon_ * total_count_) {
                // Merge: skip this tuple
                merged.push_back({summary_[i].value,
                    summary_[i].g + summary_[i+1].g,
                    summary_[i+1].delta});
                ++i;  // skip next
            } else {
                merged.push_back(summary_[i]);
            }
        }
        if (!merged.empty()) merged.back().delta = 0;
        summary_ = std::move(merged);
    }

    double epsilon_;
    size_t total_count_ = 0;
    std::vector<tuple> summary_;
};
