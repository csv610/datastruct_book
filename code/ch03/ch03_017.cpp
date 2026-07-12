#include <iostream>

int main() {
    // p(x) = 3x^3 + 2x + 1
    polynomial p({1, 2, 0, 3});
    // q(x) = x^2 + 4
    polynomial q({4, 0, 1});

    std::cout << "p(x) = " << p << "\n";
    std::cout << "q(x) = " << q << "\n";
    std::cout << "p(2) = " << p.evaluate(2) << "\n";  // 30

    polynomial sum = p.add(q);
    std::cout << "p+q = " << sum << "\n";

    polynomial prod = p.multiply(q);
    std::cout << "p*q = " << prod << "\n";

    polynomial dp = p.derivative();
    std::cout << "p'(x) = " << dp << "\n";
}
