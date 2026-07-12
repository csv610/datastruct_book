size_t hash_sdbm(std::string_view s) {
    size_t h = 0;
    for (char c : s) {
        h = c + (h << 6) + (h << 16) - h;
    }
    return h;
}
