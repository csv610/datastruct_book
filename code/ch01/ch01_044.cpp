std::string a = "hello";
std::string b = std::move(a);  // move constructor
// a is now in a valid but unspecified state
// b == "hello"
