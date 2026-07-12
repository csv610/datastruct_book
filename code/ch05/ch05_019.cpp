struct triple {
    size_t row;
    size_t col;
    double value;
};

class sparse_matrix_coo {
public:
    sparse_matrix_coo(size_t rows, size_t cols)
        : rows_(rows), cols_(cols) {}

    void add_entry(size_t row, size_t col, double value) {
        if (value != 0.0) {
            entries_.push_back({row, col, value});
        }
    }

    double operator()(size_t row, size_t col) const {
        for (const auto& e : entries_) {
            if (e.row == row && e.col == col) return e.value;
        }
        return 0.0;
    }

    size_t nonzero_count() const noexcept { return entries_.size(); }
    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }

private:
    size_t rows_, cols_;
    std::vector<triple> entries_;  // unsorted; sort by (row, col) for efficiency
};
