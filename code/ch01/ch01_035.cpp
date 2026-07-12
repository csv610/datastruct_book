#include <memory>

auto p = std::make_unique<int>(42);
// *p == 42

// Transfer ownership
auto q = std::move(p);
// p is now null, q owns the int

q.reset();  // deletes the int
