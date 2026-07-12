auto a = std::make_unique<int>(1);
// auto b = a;         // error: copy deleted
auto b = std::move(a); // OK: move
