class spell_checker {
public:
    void load_dictionary(std::istream& dict) {
        std::string word;
        while (dict >> word) {
            std::transform(word.begin(), word.end(), word.begin(),
                           ::tolower);
            tst_.insert(word);
        }
    }

    bool is_correct(std::string_view word) const {
        return tst_.search(word);
    }

    std::vector<std::string> suggest(std::string_view word) const {
        std::vector<std::string> results;
        std::string w(word);
        for (size_t i = 0; i < w.size(); ++i) {
            char orig = w[i];
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == orig) continue;
                w[i] = c;
                if (tst_.search(w)) results.push_back(w);
            }
            w[i] = orig;
        }
        return results;
    }

private:
    ternary_search_tree tst_;
};
