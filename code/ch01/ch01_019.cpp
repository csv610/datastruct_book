auto len(std::string_view s) -> size_t {
    return s.size();
}
std::string a = "hello";
len(a);         // from std::string
len("world");   // from string literal -- no allocation!
