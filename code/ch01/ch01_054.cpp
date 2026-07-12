#include <variant>
#include <string>
#include <iostream>

using Value = std::variant<int, double, std::string>;

Value v = 42;
v = "hello";

// std::holds_alternative<T>(v) checks the active type
// std::get<T>(v) extracts the value

std::visit([](auto& val) {
    std::cout << val << '\n';
}, v);
