#include <numeric> // std::gcd

struct fraction { long long num, den; };

std::vector<fraction> egyptian_fraction(long long p, long long q) {
    std::vector<fraction> result;

    while (p > 0) {
        // Find smallest k such that 1/k <= p/q, i.e., k >= q/p
        long long k = (q + p - 1) / p;  // ceil(q/p)

        result.push_back({1, k});

        // Subtract 1/k from p/q
        p = p * k - q;
        q = q * k;

        if (p == 0) break;

        // Simplify the remainder
        long long g = std::gcd(p, q);
        p /= g;
        q /= g;
    }
    return result;
}
