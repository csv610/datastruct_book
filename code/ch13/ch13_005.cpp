class autocomplete {
public:
    void add_word(std::string_view word, int frequency) {
        trie_.insert(word, frequency);
    }

    std::vector<std::string> suggest(std::string_view prefix, int max_suggestions = 5) {
        auto candidates = trie_.keys_with_prefix(prefix);
        std::sort(candidates.begin(), candidates.end(),
            [&](const std::string& a, const std::string& b) {
                return trie_.find(a).value_or(0) > trie_.find(b).value_or(0);
            });
        if (candidates.size() > max_suggestions)
            candidates.resize(max_suggestions);
        return candidates;
    }

private:
    trie<int> trie_;
};
