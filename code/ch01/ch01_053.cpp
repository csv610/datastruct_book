#include <optional>
#include <string>

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

auto result = safe_divide(10, 3);
if (result) {
    // *result == 3
}

// Default value
int value = safe_divide(10, 0).value_or(-1);  // value == -1
