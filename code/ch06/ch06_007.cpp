int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

std::string infix_to_postfix(std::string_view expr) {
    linked_stack<char> ops;
    std::string output;
    for (char c : expr) {
        if (std::isalnum(c)) {
            output += c;
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output += ops.top(); ops.pop();
            }
            ops.pop();  // discard '('
        } else { // operator
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                output += ops.top(); ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) { output += ops.top(); ops.pop(); }
    return output;
}
