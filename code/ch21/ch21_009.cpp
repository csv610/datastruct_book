class editor {
public:
    void type(char c) {
        history_.push_back(
            history_.back().append(c));
    }
    void undo() {
        if (cursor_ > 0) --cursor_;
    }
    void redo() {
        if (cursor_ + 1 < (int)history_.size()) ++cursor_;
    }
    char get(size_t idx) const {
        return history_[cursor_].get(idx);
    }
private:
    std::vector<persistent_array<char>> history_;
    int cursor_ = 0;
};
