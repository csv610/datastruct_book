#pragma once
#include <algorithm>
#include <cstddef>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace dsa {

// ---- Aho-Corasick Algorithm ----
// Multi-pattern string matching automaton
class aho_corasick {
public:
    struct node {
        std::unordered_map<char, int> children;
        int fail = 0;
        std::vector<int> output;
    };

    explicit aho_corasick(const std::vector<std::string_view>& patterns) {
        trie_.push_back({});  // root = 0
        for (int pid = 0; pid < static_cast<int>(patterns.size()); ++pid) {
            int cur = 0;
            for (char c : patterns[pid]) {
                if (trie_[cur].children.find(c) == trie_[cur].children.end()) {
                    trie_[cur].children[c] = static_cast<int>(trie_.size());
                    trie_.push_back({});
                }
                cur = trie_[cur].children[c];
            }
            trie_[cur].output.push_back(pid);
            patterns_.push_back(std::string(patterns[pid]));
        }
        build_fail();
    }

    // Search for all pattern occurrences in text
    // Returns vector of (position, pattern_id) pairs
    std::vector<std::pair<std::size_t, int>> search(std::string_view text) const {
        std::vector<std::pair<std::size_t, int>> results;
        int state = 0;
        for (std::size_t i = 0; i < text.size(); ++i) {
            while (state != 0 && trie_[state].children.find(text[i]) == trie_[state].children.end())
                state = trie_[state].fail;
            auto it = trie_[state].children.find(text[i]);
            if (it != trie_[state].children.end())
                state = it->second;
            // Output patterns from current node (already merged via fail links)
            for (int pid : trie_[state].output)
                results.push_back({i + 1 - patterns_[pid].size(), pid});
        }
        return results;
    }

    // Count total pattern occurrences
    std::size_t count(std::string_view text) const {
        return search(text).size();
    }

private:
    void build_fail() {
        std::queue<int> q;
        for (auto& [c, child] : trie_[0].children) {
            trie_[child].fail = 0;
            q.push(child);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& [c, v] : trie_[u].children) {
                int f = trie_[u].fail;
                while (f != 0 && trie_[f].children.find(c) == trie_[f].children.end())
                    f = trie_[f].fail;
                auto it = trie_[f].children.find(c);
                trie_[v].fail = (it != trie_[f].children.end() && it->second != v) ? it->second : 0;
                // Merge output lists
                auto& out = trie_[v].output;
                for (int pid : trie_[trie_[v].fail].output)
                    out.push_back(pid);
                q.push(v);
            }
        }
    }

    std::vector<node> trie_;
    std::vector<std::string> patterns_;
};

// ---- Commentz-Walter Algorithm ----
// Multi-pattern matching using Trie + shift (generalization of Boyer-Moore)
class commentz_walter {
public:
    struct trie_node {
        std::unordered_map<char, int> children;
        int pattern_id = -1;
        int depth = 0;
    };

    explicit commentz_walter(const std::vector<std::string_view>& patterns)
        : patterns_(patterns.begin(), patterns.end()) {
        trie_.push_back({});  // root = 0
        max_depth_ = 0;
        for (int pid = 0; pid < static_cast<int>(patterns.size()); ++pid) {
            int cur = 0;
            for (char c : patterns[pid]) {
                if (trie_[cur].children.find(c) == trie_[cur].children.end()) {
                    trie_[cur].children[c] = static_cast<int>(trie_.size());
                    trie_.push_back({});
                    trie_.back().depth = trie_[cur].depth + 1;
                }
                cur = trie_[cur].children[c];
            }
            trie_[cur].pattern_id = pid;
            max_depth_ = std::max(max_depth_, static_cast<int>(patterns[pid].size()));
        }

        // Bad character shifts
        bad_char_.fill(max_depth_);
        for (int pid = 0; pid < static_cast<int>(patterns.size()); ++pid) {
            for (std::size_t i = 0; i < patterns[pid].size() - 1; ++i)
                bad_char_[static_cast<unsigned char>(patterns[pid][i])] =
                    std::max(1, static_cast<int>(patterns[pid].size()) - static_cast<int>(i) - 1);
        }
    }

    std::vector<std::pair<std::size_t, int>> search(std::string_view text) const {
        std::vector<std::pair<std::size_t, int>> results;
        std::size_t n = text.size();
        if (n == 0 || max_depth_ == 0) return results;

        for (std::size_t s = 0; s < n;) {
            int node = 0;
            std::size_t j = 0;
            std::size_t limit = std::min(n, s + max_depth_);
            std::vector<int> nodes_at_depth;

            while (s + j < limit) {
                auto it = trie_[node].children.find(text[s + j]);
                if (it == trie_[node].children.end()) break;
                node = it->second;
                nodes_at_depth.push_back(node);
                ++j;
            }

            // Check for matches (shortest patterns first)
            for (int k = static_cast<int>(nodes_at_depth.size()) - 1; k >= 0; --k) {
                int nid = nodes_at_depth[k];
                if (trie_[nid].pattern_id >= 0) {
                    int pid = trie_[nid].pattern_id;
                    std::size_t plen = patterns_[pid].size();
                    if (s + plen <= n && j >= plen) {
                        results.push_back({s, pid});
                    }
                }
            }

            if (!nodes_at_depth.empty()) {
                // Found at least one character, advance by 1
                s += 1;
            } else {
                // No match at all, use bad character shift
                std::size_t shift = bad_char_[static_cast<unsigned char>(text[s])];
                s += std::max(std::size_t(1), shift);
            }
        }
        return results;
    }

private:
    std::vector<trie_node> trie_;
    std::vector<std::string> patterns_;
    std::array<int, 256> bad_char_{};
    int max_depth_ = 0;
};

}  // namespace dsa
