#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(5);
// No delete needed -- freed automatically when p goes out of scope
