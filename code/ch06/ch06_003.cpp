bool is_balanced(std::string_view expr) {
    linked_stack<char> s;
    for (char c : expr) {
        switch (c) {
            case '(': case '[': case '{':
                s.push(c);
                break;
            case ')':
                if (s.empty() || s.top() != '(') return false;
                s.pop();
                break;
            case ']':
                if (s.empty() || s.top() != '[') return false;
                s.pop();
                break;
            case '}':
                if (s.empty() || s.top() != '{') return false;
                s.pop();
                break;
        }
    }
    return s.empty();
}
