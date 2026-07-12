int offset = 10;
int factor = 2;

// By value -- copy
auto by_val = [offset, factor](int x) {
    return x * factor + offset;
};

// By reference -- no copy, can modify
auto by_ref = [&offset, &factor](int x) {
    factor = 1;
    return x + offset;
};

// Capture all by value
auto all_val = [=](int x) { return x + offset; };

// Capture all by reference
auto all_ref = [&](int x) { offset += x; };

// Mixed
auto mixed = [=, &offset](int x) { return x + offset; };
