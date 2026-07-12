double eval_rpn(const std::vector<std::string>& tokens) {
    std::stack<double> s;
    for (const auto& t : tokens) {
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            double b = s.top(); s.pop();
            double a = s.top(); s.pop();
            if (t == "+") s.push(a + b);
            else if (t == "-") s.push(a - b);
            else if (t == "*") s.push(a * b);
            else s.push(a / b);
        } else {
            s.push(std::stod(t));
        }
    }
    return s.top();
}
