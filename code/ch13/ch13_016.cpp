struct suffix_tree {
    struct edge {
        int start, end;  // edge label = s[start..end]
        int child;
    };

    struct node {
        std::map<char, edge> children;
        int suffix_link = 0;  // points to node representing suffix
        int depth = 0;
    };

    std::string s;
    std::vector<node> nodes;

    explicit suffix_tree(const std::string& str) : s(str) {
        nodes.emplace_back();  // root = node 0
        build_ukkonen();
    }

    // Find all occurrences of pattern in O(m) time
    bool contains(std::string_view pattern) const {
        int u = 0;
        size_t i = 0;
        while (i < pattern.size()) {
            auto it = nodes[u].children.find(pattern[i]);
            if (it == nodes[u].children.end()) return false;
            const auto& e = it->second;
            int len = e.end - e.start + 1;
            int remaining = pattern.size() - i;
            int match = std::min(len, remaining);
            for (int j = 0; j < match; ++j) {
                if (s[e.start + j] != pattern[i + j]) return false;
            }
            i += match;
            if (match < len) return i == pattern.size();  // partial edge
            u = e.child;
        }
        return true;
    }

    // Count distinct substrings: sum of (depth[v] - depth[parent]) for all leaves
    size_t count_distinct_substrings() const {
        size_t count = 0;
        for (size_t u = 1; u < nodes.size(); ++u) {
            // u is a leaf if it has no children
            if (nodes[u].children.empty()) {
                count += nodes[u].depth;
            }
        }
        return count;
    }

private:
    int active_node = 0, active_edge = 0, active_length = 0;
    int remaining = 0, leaf_end = -1;
    int size_ = 0;

    void extend(int pos) {
        leaf_end = pos;
        ++remaining;

        int last_internal = -1;
        while (remaining > 0) {
            if (active_length == 0) active_edge = pos;

            if (!nodes[active_node].children.count(s[active_edge])) {
                // Rule 1: no edge starting with active_edge -- create new edge
                nodes[active_node].children[s[active_edge]] = {pos, leaf_end, ++size_};
                nodes.emplace_back();
                nodes.back().depth = nodes[active_node].depth + (leaf_end - pos + 1);
                if (last_internal != -1) {
                    nodes[last_internal].suffix_link = active_node;
                    last_internal = -1;
                }
            } else {
                int split = nodes[active_node].children[s[active_edge]].start;
                int split_end = split + active_length - 1;

                if (active_length > 0) {
                    // Rule 2: split edge
                    node new_node;
                    new_node.depth = nodes[active_node].depth + active_length;
                    nodes.push_back(new_node);
                    int new_idx = size_ + 1;

                    auto& e = nodes[active_node].children[s[active_edge]];
                    e.end = split_end;
                    nodes[e.child].children[s[split_end + 1]] = {split_end + 1, e.end, e.child};
                    e.child = new_idx;

                    nodes[new_idx].children[s[pos]] = {pos, leaf_end, ++size_};
                    nodes.emplace_back();
                    nodes.back().depth = new_node.depth + (leaf_end - pos + 1);

                    if (last_internal != -1) nodes[last_internal].suffix_link = new_idx;
                    last_internal = new_idx;
                }
            }

            --remaining;
            if (remaining == 0) break;

            // Follow suffix link
            if (active_node == 0 && active_length > 0) {
                --active_length;
                active_edge = pos - remaining + 1;
            } else {
                active_node = nodes[active_node].suffix_link;
            }
        }
    }

    void build_ukkonen() {
        for (int i = 0; i < static_cast<int>(s.size()); ++i)
            extend(i);
    }
};
