int word_ladder(const std::string& begin,
                const std::string& end,
                const std::unordered_set<std::string>& dict)
{
    if (begin == end) return 0;

    std::queue<std::pair<std::string,int>> bfs;
    std::unordered_set<std::string> visited;
    bfs.push({begin, 1});
    visited.insert(begin);

    while (!bfs.empty()) {
        auto [word, steps] = bfs.front();
        bfs.pop();

        // Try changing each character.
        for (int i = 0; i < (int)word.size(); ++i) {
            char original = word[i];
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == original) continue;
                word[i] = c;
                if (word == end) return steps + 1;
                if (dict.count(word) &&
                    !visited.count(word))
                {
                    visited.insert(word);
                    bfs.push({word, steps + 1});
                }
            }
            word[i] = original;  // restore
        }
    }
    return 0;  // no ladder found
}
