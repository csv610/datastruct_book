#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dsa {

// ---- de Bruijn Graph ----
// Nodes are (k-1)-mers, edges are k-mers
class de_bruijn_graph {
public:
    explicit de_bruijn_graph(int k) : k_(k) {}

    // Add an edge (k-mer) to the graph
    void add_edge(std::string_view kmer) {
        if (static_cast<int>(kmer.size()) != k_) return;
        std::string from(kmer.substr(0, k_ - 1));
        std::string to(kmer.substr(1, k_ - 1));
        adj_[from].push_back(to);
        in_degree_[to]++;
        in_degree_[from];  // ensure entry exists
        nodes_.insert(from);
        nodes_.insert(to);
    }

    // Build from a sequence (decompose into k-mers)
    void add_sequence(std::string_view seq) {
        if (static_cast<int>(seq.size()) < k_) return;
        for (std::size_t i = 0; i + k_ <= seq.size(); ++i)
            add_edge(seq.substr(i, k_));
    }

    // Build from a set of reads
    void add_reads(const std::vector<std::string>& reads) {
        for (const auto& r : reads)
            add_sequence(r);
    }

    // Find an Eulerian path (Hierholzer's algorithm)
    // Returns the path as a sequence of (k-1)-mers, or empty if none exists
    std::vector<std::string> eulerian_path() const {
        // Check Eulerian path conditions
        std::string start_node;
        int start_excess = 0, end_excess = 0;
        bool has_path = false;

        for (const auto& node : nodes_) {
            int out = out_degree(node);
            int in = in_degree_val(node);
            if (out - in == 1) {
                start_node = node;
                start_excess++;
                has_path = true;
            } else if (in - out == 1) {
                end_excess++;
            } else if (out != in) {
                return {};  // No Eulerian path
            }
        }

        if (start_excess > 1 || end_excess > 1) return {};
        if (!has_path) {
            // For Eulerian cycle, start anywhere with edges
            for (const auto& node : nodes_) {
                if (out_degree(node) > 0) {
                    start_node = node;
                    break;
                }
            }
        }
        if (start_node.empty()) return {};

        // Hierholzer's algorithm
        std::unordered_map<std::string, std::vector<std::string>> adj = adj_;
        std::vector<std::string> stack, path;
        stack.push_back(start_node);

        while (!stack.empty()) {
            const auto& u = stack.back();
            auto it = adj.find(u);
            if (it != adj.end() && !it->second.empty()) {
                std::string v = it->second.back();
                it->second.pop_back();
                stack.push_back(v);
            } else {
                path.push_back(u);
                stack.pop_back();
            }
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    // Reconstruct sequence from Eulerian path
    std::string reconstruct(const std::vector<std::string>& path) const {
        if (path.empty()) return "";
        std::string result = path[0];
        for (std::size_t i = 1; i < path.size(); ++i)
            result += path[i].back();
        return result;
    }

    // Simple greedy genome assembly (no error correction)
    std::string assemble() const {
        auto path = eulerian_path();
        return reconstruct(path);
    }

    // Get all nodes
    const std::unordered_set<std::string>& nodes() const { return nodes_; }

    int out_degree(const std::string& node) const {
        auto it = adj_.find(node);
        return (it != adj_.end()) ? static_cast<int>(it->second.size()) : 0;
    }

    int in_degree_val(const std::string& node) const {
        auto it = in_degree_.find(node);
        return (it != in_degree_.end()) ? it->second : 0;
    }

    std::size_t edge_count() const {
        std::size_t count = 0;
        for (const auto& [_, edges] : adj_)
            count += edges.size();
        return count;
    }

private:
    int k_;
    std::unordered_map<std::string, std::vector<std::string>> adj_;
    std::unordered_map<std::string, int> in_degree_;
    std::unordered_set<std::string> nodes_;
};

// ---- de Bruijn Sequence (classical) ----
// Generates a de Bruijn sequence B(k, n) over alphabet of size k
// containing all n-mers exactly once
inline std::string de_bruijn_sequence(int alphabet_size, int n) {
    std::string sequence;
    std::vector<int> a(alphabet_size * n, 0);

    auto db = [&](auto&& self, int t, int p) -> void {
        if (t > n) {
            if (n % p == 0)
                for (int j = 1; j <= p; ++j)
                    sequence += static_cast<char>('0' + a[j]);
        } else {
            a[t] = a[t - p];
            self(self, t + 1, p);
            for (int j = a[t - p] + 1; j < alphabet_size; ++j) {
                a[t] = j;
                self(self, t + 1, t);
            }
        }
    };

    db(db, 1, 1);
    return sequence;
}

}  // namespace dsa
