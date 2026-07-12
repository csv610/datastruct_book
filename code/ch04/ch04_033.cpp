class editor_buffer {
public:
    void insert(const std::string& text, size_t pos) {
        auto [left, right] = buffer_.split(pos);
        rope mid(text);
        left.concatenate(std::move(mid));
        left.concatenate(std::move(right));
        buffer_ = std::move(left);
    }

    void erase(size_t pos, size_t len) {
        auto [left, mid_right] = buffer_.split(pos);
        auto [mid, right] = mid_right.split(len);
        left.concatenate(std::move(right));
        buffer_ = std::move(left);
    }

    std::string get_text() const { return buffer_.to_string(); }

private:
    rope buffer_;
};
