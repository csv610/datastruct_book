int x = 5;         // x is an lvalue, 5 is an rvalue
int y = x + 2;     // (x + 2) is an rvalue
int z = std::move(x); // std::move(x) casts x to an rvalue
