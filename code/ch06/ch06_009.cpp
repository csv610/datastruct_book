struct parse_error {
    int line;
    int column;
    char expected;
    char found;
    parse_error(int l, int c, char e, char f)
        : line(l), column(c), expected(e), found(f) {}
};

std::vector<parse_error> check_balanced(std::string_view src) {
    std::vector<parse_error> errors;
    linked_stack<std::pair<char, std::pair<int, int>>> stk;
    int line = 1, col = 0;
    for (char c : src) {
        ++col;
        if (c == '\n') { ++line; col = 0; continue; }
        if (c == '(' || c == '[' || c == '{') {
            stk.push({c, {line, col}});
        } else if (c == ')' || c == ']' || c == '}') {
            if (stk.empty()) {
                errors.emplace_back(line, col, '\0', c);
            } else {
                auto [open, pos] = stk.top();
                stk.pop();
                char match = (c == ')') ? '('
                           : (c == ']') ? '[' : '{';
                if (open != match) {
                    errors.emplace_back(
                        line, col, match, c);
                }
            }
        }
    }
    while (!stk.empty()) {
        auto [open, pos] = stk.top();
        stk.pop();
        char expected_close = (open == '(') ? ')'
                            : (open == '[') ? ']' : '}';
        errors.emplace_back(
            pos.first, pos.second,
            expected_close, '\0');
    }
    return errors;
}
