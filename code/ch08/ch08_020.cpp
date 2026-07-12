std::vector<int> lzw_compress(std::string_view input) {
    // Initialize dictionary with single characters
    hash_table_chaining<std::string, int> dict;
    for (int i = 0; i < 256; ++i) {
        dict.insert(std::string(1, static_cast<char>(i)), i);
    }

    std::vector<int> output;
    std::string w;
    int next_code = 256;

    for (char c : input) {
        std::string wc = w + c;
        if (dict.find(wc).has_value()) {
            w = wc;
        } else {
            output.push_back(*dict.find(w));
            dict.insert(wc, next_code++);
            w = std::string(1, c);
        }
    }
    if (!w.empty()) output.push_back(*dict.find(w));
    return output;
}
