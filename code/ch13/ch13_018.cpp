struct RadixNode {
    std::map<char, std::pair<std::string_view,
                             RadixNode*>> edges;
    bool is_end = false;
};

struct RadixTree {
    RadixNode* root = new RadixNode();

    void insert(std::string_view key) {
        RadixNode* node = root;
        size_t i = 0;
        while (i < key.size()) {
            char c = key[i];
            if (node->edges.count(c) == 0) {
                // No matching edge: create one
                node->edges[c] = {key.substr(i),
                                  new RadixNode()};
                node->edges[c].second->is_end = true;
                return;
            }
            auto& [label, child] = node->edges[c];
            // Find length of common prefix
            size_t j = 0;
            while (j < label.size() && i + j < key.size()
                   && label[j] == key[i + j])
                ++j;
            if (j == label.size()) {
                // Full label match: descend
                i += j;
                node = child;
                continue;
            }
            // Partial match: split the edge
            RadixNode* mid = new RadixNode();
            node->edges[c] = {
                label.substr(0, j), mid};
            mid->edges[label[j]] = {
                label.substr(j), child};
            size_t remaining = key.size() - i - j;
            if (remaining == 0) {
                mid->is_end = true;
            } else {
                mid->edges[key[i + j]] = {
                    key.substr(i + j),
                    new RadixNode()};
                mid->edges[key[i+j]].second->is_end
                    = true;
            }
            return;
        }
        node->is_end = true;
    }

    bool search(std::string_view key) const {
        RadixNode* node = root;
        size_t i = 0;
        while (i < key.size()) {
            char c = key[i];
            if (node->edges.count(c) == 0)
                return false;
            auto& [label, child] = node->edges.at(c);
            if (key.substr(i).substr(0, label.size())
                != label)
                return false;
            i += label.size();
            node = child;
        }
        return node->is_end;
    }

    bool prefix_search(
        std::string_view prefix) const {
        RadixNode* node = root;
        size_t i = 0;
        while (i < prefix.size()) {
            char c = prefix[i];
            if (node->edges.count(c) == 0)
                return false;
            auto& [label, child] = node->edges.at(c);
            if (prefix.substr(i).substr(0, label.size())
                != label)
                return false;
            i += label.size();
            node = child;
        }
        return true;
    }
};
