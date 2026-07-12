int evaluate_postfix(std::string_view expr) {
    linked_stack<int> s;
    for (char c : expr) {
        if (std::isspace(c)) continue;
        if (std::isdigit(c)) {
            s.push(c - '0');
        } else {
            int b = s.top(); s.pop();
            int a = s.top(); s.pop();
            switch (c) {
                case '+': s.push(a + b); break;
                case '-': s.push(a - b); break;
                case '*': s.push(a * b); break;
                case '/': s.push(a / b); break;
            }
        }
    }
    return s.top();
}
