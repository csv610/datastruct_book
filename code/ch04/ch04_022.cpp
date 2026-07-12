struct term {
    int exponent;
    double coefficient;
};

using polynomial = linked_list<term>;

linked_list<term> add_polynomials(const linked_list<term>& a,
                                  const linked_list<term>& b) {
    linked_list<term> result;
    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) {
        if (it_a->exponent == it_b->exponent) {
            double sum = it_a->coefficient + it_b->coefficient;
            if (sum != 0.0) {
                result.push_back({it_a->exponent, sum});
            }
            ++it_a; ++it_b;
        } else if (it_a->exponent > it_b->exponent) {
            result.push_back({it_a->exponent, it_a->coefficient});
            ++it_a;
        } else {
            result.push_back({it_b->exponent, it_b->coefficient});
            ++it_b;
        }
    }

    while (it_a != a.end()) {
        result.push_back({it_a->exponent, it_a->coefficient});
        ++it_a;
    }
    while (it_b != b.end()) {
        result.push_back({it_b->exponent, it_b->coefficient});
        ++it_b;
    }

    return result;
}
