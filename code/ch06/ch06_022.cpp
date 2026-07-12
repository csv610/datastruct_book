#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <cctype>

int prefix_prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool is_right_assoc(char op) { return op == '^'; }

std::string infix_to_prefix(std::string_view expr) {
    std::stack<char> ops;
    std::vector<std::string> out;

    for (int i = expr.size() - 1; i >= 0; --i) {
        char c = expr[i];
        if (std::isalnum(c)) {
            out.push_back(std::string(1, c));
        } else if (c == ')') {
            ops.push(c);
        } else if (c == '(') {
            while (!ops.empty() && ops.top() != ')') {
                out.push_back(std::string(1, ops.top()));
                ops.pop();
            }
            if (!ops.empty()) ops.pop();
        } else { // operator
            while (!ops.empty() && ops.top() != ')' &&
                   (prefix_prec(ops.top()) > prefix_prec(c) ||
                    (prefix_prec(ops.top()) == prefix_prec(c) &&
                     !is_right_assoc(c)))) {
                out.push_back(std::string(1, ops.top()));
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        out.push_back(std::string(1, ops.top()));
        ops.pop();
    }

    std::reverse(out.begin(), out.end());
    std::string result;
    for (size_t i = 0; i < out.size(); ++i) {
        if (i) result += ' ';
        result += out[i];
    }
    return result;
}
