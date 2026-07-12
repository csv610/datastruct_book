struct social_graph {
    graph_list<> connections;
    std::vector<std::string> names;

    // Degrees of separation from a person
    std::vector<size_t> degrees_of_separation(const std::string& person) {
        auto it = std::find(names.begin(), names.end(), person);
        if (it == names.end()) return {};
        return bfs(connections, it - names.begin());
    }

    // Friend recommendations (friends of friends not already connected)
    std::vector<std::string> recommendations(const std::string& person) {
        auto it = std::find(names.begin(), names.end(), person);
        if (it == names.end()) return {};
        size_t idx = it - names.begin();

        std::unordered_set<size_t> friends;
        for (const auto& [to, _] : connections.neighbors(idx)) {
            friends.insert(to);
        }

        std::unordered_set<size_t> candidates;
        for (size_t f : friends) {
            for (const auto& [to, _] : connections.neighbors(f)) {
                if (to != idx && !friends.count(to)) {
                    candidates.insert(to);
                }
            }
        }

        std::vector<std::string> result;
        for (size_t c : candidates) result.push_back(names[c]);
        return result;
    }
};
