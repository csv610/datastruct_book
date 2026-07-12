#include <string>
#include <vector>
#include <stdexcept>

double eval_postfix(const std::vector<std::string>& tokens) {
    linked_stack<double> stk;
    for (const auto& tok : tokens) {
        if (tok == "+" || tok == "-" ||
            tok == "*" || tok == "/") {
            if (stk.size() < 2)
                throw std::runtime_error("bad expr");
            double b = stk.top(); stk.pop();
            double a = stk.top(); stk.pop();
            if (tok == "+") stk.push(a + b);
            else if (tok == "-") stk.push(a - b);
            else if (tok == "*") stk.push(a * b);
            else stk.push(a / b);
        } else {
            stk.push(std::stod(tok));
        }
    }
    if (stk.size() != 1)
        throw std::runtime_error("bad expr");
    return stk.top();
}
