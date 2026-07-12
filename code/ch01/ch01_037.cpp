auto s1 = std::make_shared<std::string>("hello");
auto s2 = s1;  // ref count = 2
// s1 and s2 both point to "hello"
s2.reset();    // ref count = 1
// "hello" still alive (s1 owns it)
