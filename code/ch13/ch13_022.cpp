#include <array>
#include <queue>
#include <string>
#include <vector>

struct aho_corasick {
    static constexpr int ALPHA = 26;

    struct node {
        std::array<int, ALPHA> next{};
        std::array<int, ALPHA> go{};
        int link = 0;
        int dict_link = -1;
        int pattern_id = -1;
        node() { next.fill(-1); go.fill(-1); }
    };

    std::vector<node> t = {node()};

    void add_pattern(const std::string& pat, int id) {
        int v = 0;
        for (char c : pat) {
            int ch = c - 'a';
            if (t[v].next[ch] == -1) {
                t[v].next[ch] = t.size();
                t.emplace_back();
            }
            v = t[v].next[ch];
        }
        t[v].pattern_id = id;
    }

    void build() {
        std::queue<int> q;
        for (int c = 0; c < ALPHA; ++c) {
            if (t[0].next[c] != -1) {
                t[0].go[c] = t[0].next[c];
                q.push(t[0].next[c]);
            } else {
                t[0].go[c] = 0;
            }
        }
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int c = 0; c < ALPHA; ++c) {
                if (t[v].next[c] != -1) {
                    t[t[v].link].go[c] == -1
                        ? t[v].go[c] = t[v].next[c]
                        : t[v].go[c] = t[t[v].link].go[c];
                    t[t[v].next[c]].link =
                        (v == 0) ? 0
                                 : t[t[v].link].go[c];
                    if (t[t[t[v].next[c]].link].pattern_id
                            != -1)
                        t[t[v].next[c]].dict_link =
                            t[t[v].next[c]].link;
                    else
                        t[t[v].next[c]].dict_link =
                            t[t[t[v].next[c]].link]
                                .dict_link;
                    q.push(t[v].next[c]);
                }
            }
        }
    }
