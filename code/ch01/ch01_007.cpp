auto p1 = std::make_unique<int>(5);
// auto p2 = p1;  // error: cannot copy unique_ptr
auto p2 = std::move(p1);  // OK: transfer ownership
// p1 is now null
