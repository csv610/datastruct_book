bool is_balanced(std::string_view s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') { st.push(c); continue; }
        if (st.empty()) return false;
        char t = st.top(); st.pop();
        if ((c == ')' && t != '(') || (c == ']' && t != '[') || (c == '}' && t != '{'))
            return false;
    }
    return st.empty();
}
