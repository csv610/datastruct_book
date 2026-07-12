// djb2 hash -- simple but effective
size_t hash_string(std::string_view s) {
    size_t h = 5381;
    for (char c : s) {
        h = h * 33 + static_cast<size_t>(c);
    }
    return h;
}
