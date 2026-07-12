auto s1 = std::make_shared<int>(5);
auto s2 = s1;  // OK: both now point to the same int
// Memory freed when both s1 and s2 go out of scope
