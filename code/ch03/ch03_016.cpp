#include <vector>
#include <stdexcept>
#include <iostream>

class polynomial {
public:
    polynomial() = default;

    explicit polynomial(std::vector<double> coeffs)
        : coeffs_(std::move(coeffs)) {
        trim();
    }

    int degree() const {
        return static_cast<int>(coeffs_.size()) - 1;
    }

    bool is_zero() const {
        return coeffs_.empty();
    }

    double coefficient(int i) const {
        if (i < 0 || i > degree()) return 0.0;
        return coeffs_[i];
    }

    // Horner's method: O(n) evaluation
    double evaluate(double x) const {
        if (is_zero()) return 0.0;
        double result = coeffs_[degree()];
        for (int i = degree() - 1; i >= 0; --i) {
            result = result * x + coeffs_[i];
        }
        return result;
    }

    polynomial add(const polynomial& q) const {
        size_t n = std::max(coeffs_.size(), q.coeffs_.size());
        std::vector<double> result(n, 0.0);
        for (size_t i = 0; i < coeffs_.size(); ++i) result[i] += coeffs_[i];
        for (size_t i = 0; i < q.coeffs_.size(); ++i) result[i] += q.coeffs_[i];
        return polynomial(std::move(result));
    }

    // Naive polynomial multiplication: O(n * m)
    polynomial multiply(const polynomial& q) const {
        if (is_zero() || q.is_zero()) return polynomial();
        int n = degree();
        int m = q.degree();
        std::vector<double> result(n + m + 1, 0.0);
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= m; ++j) {
                result[i + j] += coeffs_[i] * q.coeffs_[j];
            }
        }
        return polynomial(std::move(result));
    }

    // Returns the formal derivative: O(n)
    polynomial derivative() const {
        if (degree() <= 0) return polynomial();
        std::vector<double> result(degree());
        for (int i = 1; i <= degree(); ++i) {
            result[i - 1] = i * coeffs_[i];
        }
        return polynomial(std::move(result));
    }

    bool operator==(const polynomial& q) const {
        return coeffs_ == q.coeffs_;
    }

    bool operator!=(const polynomial& q) const {
        return !(*this == q);
    }

    friend std::ostream& operator<<(std::ostream& os, const polynomial& p) {
        if (p.is_zero()) return os << "0";
        for (int i = p.degree(); i >= 0; --i) {
            if (p.coeffs_[i] == 0.0) continue;
            if (i < p.degree()) os << (p.coeffs_[i] > 0 ? " + " : " - ");
            double c = std::abs(p.coeffs_[i]);
            if (c != 1.0 || i == 0) os << c;
            if (i > 0) os << "x";
            if (i > 1) os << "^" << i;
        }
        return os;
    }

private:
    // Remove trailing zero coefficients
    void trim() {
        while (!coeffs_.empty() && coeffs_.back() == 0.0) {
            coeffs_.pop_back();
        }
    }

    std::vector<double> coeffs_;
};
