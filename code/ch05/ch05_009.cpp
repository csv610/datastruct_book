class gap_buffer {
public:
    explicit gap_buffer(size_t initial = 128)
        : buf_(initial), gap_start_(0), gap_end_(initial) {}

    // Insert character at cursor position -- O(1) amortized
    void insert(char c) {
        if (gap_start_ == gap_end_) grow_gap();
        buf_[gap_start_++] = c;
    }

    // Delete character before cursor -- O(1)
    char delete_back() {
        if (gap_start_ == 0) return '\0';
        return buf_[--gap_start_];
    }

    // Move cursor left -- O(1)
    void move_left() {
        if (gap_start_ > 0)
            buf_[--gap_end_] = buf_[--gap_start_];
    }

    // Move cursor right -- O(1)
    void move_right() {
        if (gap_end_ < buf_.size())
            buf_[gap_start_++] = buf_[gap_end_++];
    }

    // Get full text
    std::string text() const {
        return std::string(buf_.begin(), buf_.begin() + gap_start_) +
               std::string(buf_.begin() + gap_end_, buf_.end());
    }

    size_t cursor_pos() const { return gap_start_; }

private:
    void grow_gap() {
        size_t new_size = buf_.size() * 2;
        std::vector<char> new_buf(new_size);
        // Copy before gap
        std::copy(buf_.begin(), buf_.begin() + gap_start_, new_buf.begin());
        // Copy after gap
        size_t after = buf_.size() - gap_end_;
        size_t new_gap_end = new_size - after;
        std::copy(buf_.begin() + gap_end_, buf_.end(), new_buf.begin() + new_gap_end);
        gap_end_ = new_gap_end;
        buf_ = std::move(new_buf);
    }

    std::vector<char> buf_;
    size_t gap_start_, gap_end_;
};
