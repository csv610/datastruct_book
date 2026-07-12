#include <string>
#include <stack>
#include <functional>
#include <iostream>

struct action {
    std::string description;
    std::function<void()> do_fn;
    std::function<void()> undo_fn;
};

class text_editor {
public:
    void execute(action act) {
        act.do_fn();
        undo_stack_.push(std::move(act));
        // new action invalidates redo history
        while (!redo_stack_.empty()) redo_stack_.pop();
    }

    bool undo() {
        if (undo_stack_.empty()) return false;
        action act = std::move(undo_stack_.top());
        undo_stack_.pop();
        act.undo_fn();
        redo_stack_.push(std::move(act));
        return true;
    }

    bool redo() {
        if (redo_stack_.empty()) return false;
        action act = std::move(redo_stack_.top());
        redo_stack_.pop();
        act.do_fn();
        undo_stack_.push(std::move(act));
        return true;
    }

    bool can_undo() const { return !undo_stack_.empty(); }
    bool can_redo() const { return !redo_stack_.empty(); }

private:
    std::stack<action> undo_stack_;
    std::stack<action> redo_stack_;
};
