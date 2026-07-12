class polynomial {
public:
    polynomial() : coeff_(1) {}  // zero polynomial

    void set_coefficient(int exponent, double coefficient) {
        while (static_cast<int>(coeff_.size()) <= exponent) {
            coeff_.push_back(0.0);
        }
        coeff_[exponent] = coefficient;
    }

    double evaluate(double x) const {
        double result = 0.0;
        double power = 1.0;
        for (size_t i = 0; i < coeff_.size(); ++i) {
            result += coeff_[i] * power;
            power *= x;
        }
        return result;
    }

    double coefficient(int exponent) const {
        if (exponent >= static_cast<int>(coeff_.size())) return 0.0;
        return coeff_[exponent];
    }

    int degree() const {
        for (int i = static_cast<int>(coeff_.size()) - 1; i >= 0; --i) {
            if (coeff_[i] != 0.0) return i;
        }
        return -1;  // zero polynomial
    }

private:
    dynamic_array_list<double> coeff_;
};
