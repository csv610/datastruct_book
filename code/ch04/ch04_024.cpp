struct edit_action {
    std::string description;
    std::string previous_text;
    std::string new_text;
    size_t position;
};

class undo_manager {
public:
    void record_action(edit_action action) {
        while (current_ != undo_list_.end()) {
            current_ = undo_list_.erase(current_);
        }
        undo_list_.push_back(std::move(action));
        current_ = undo_list_.end();

        if (undo_list_.size() > max_history_) {
            undo_list_.pop_front();
        }
    }

    edit_action undo() {
        if (current_ == undo_list_.begin()) {
            throw std::runtime_error("nothing to undo");
        }
        --current_;
        return *current_;
    }

    edit_action redo() {
        if (current_ == undo_list_.end()) {
            throw std::runtime_error("nothing to redo");
        }
        auto action = *current_;
        ++current_;
        return action;
    }

private:
    linked_list<edit_action> undo_list_;
    linked_list<edit_action>::iterator current_;
    static constexpr size_t max_history_ = 100;
};
