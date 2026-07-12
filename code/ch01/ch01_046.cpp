template <typename T>
void wrapper(T&& arg) {
    // arg is a forwarding reference
    target(std::forward<T>(arg));
}

void target(int& x)  { /* lvalue path */ }
void target(int&& x) { /* rvalue path */ }

int x = 5;
wrapper(x);           // calls target(int&)
wrapper(10);          // calls target(int&&)
