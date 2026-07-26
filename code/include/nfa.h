#pragma once
#include <set>
#include <stack>
#include <string>
#include <vector>

namespace dsa {

// ---- NFA for regular expression matching (Thompson's construction) ----
// Supports: literal characters, . (any), * (Kleene star), | (alternation), () grouping
class regex_nfa {
public:
    explicit regex_nfa(const std::string& re) {
        trans_.push_back({});  // state 0 = start
        build(re);
    }

    bool matches(const std::string& text) const {
        std::set<int> reachable = eps_closure({0});
        for (char c : text) {
            std::set<int> next;
            for (int s : reachable)
                for (auto [ch, to] : trans_[s])
                    if (ch == c || ch == '.')
                        next.insert(to);
            reachable = eps_closure(next);
        }
        return reachable.count(accept_) > 0;
    }

private:
    std::vector<std::vector<std::pair<char, int>>> trans_;
    int accept_ = 0;

    int new_state() {
        trans_.push_back({});
        return static_cast<int>(trans_.size()) - 1;
    }

    void add_edge(int from, int to, char ch = '\0') {
        trans_[from].push_back({ch, to});
    }

    std::set<int> eps_closure(std::set<int> states) const {
        std::stack<int> stk;
        for (int s : states) stk.push(s);
        while (!stk.empty()) {
            int s = stk.top(); stk.pop();
            for (auto [ch, to] : trans_[s])
                if (ch == '\0' && states.insert(to).second)
                    stk.push(to);
        }
        return states;
    }

    // Returns (start, end) of fragment
    std::pair<int, int> build_fragment(const std::string& re, std::size_t& i) {
        int start = -1, end = -1;

        while (i < re.size() && re[i] != ')' && re[i] != '|') {
            if (re[i] == '(') {
                ++i;
                auto [fs, fe] = build_fragment(re, i);
                if (i < re.size() && re[i] == ')') ++i;
                // Handle quantifier after group
                if (i < re.size() && (re[i] == '*' || re[i] == '+' || re[i] == '?')) {
                    auto [qs, qe] = wrap_quantifier(fs, fe, re[i]);
                    ++i;
                    fs = qs; fe = qe;
                }
                if (start == -1) { start = fs; end = fe; }
                else { add_edge(end, fs); end = fe; }

            } else if (re[i] == '\\') {
                ++i;
                int s = new_state(), e = new_state();
                char ch = (i < re.size()) ? re[i] : '\\';
                add_edge(s, e, ch);
                ++i;
                if (i < re.size() && (re[i] == '*' || re[i] == '+' || re[i] == '?')) {
                    auto [qs, qe] = wrap_quantifier(s, e, re[i]);
                    ++i;
                    s = qs; e = qe;
                }
                if (start == -1) { start = s; end = e; }
                else { add_edge(end, s); end = e; }

            } else {
                // Literal or '.'
                char ch = re[i];
                int s = new_state(), e = new_state();
                add_edge(s, e, ch);
                ++i;
                // Handle quantifier
                if (i < re.size() && (re[i] == '*' || re[i] == '+' || re[i] == '?')) {
                    auto [qs, qe] = wrap_quantifier(s, e, re[i]);
                    ++i;
                    s = qs; e = qe;
                }
                if (start == -1) { start = s; end = e; }
                else { add_edge(end, s); end = e; }
            }
        }
        return {start, end};
    }

    std::pair<int, int> wrap_quantifier(int s, int e, char q) {
        if (q == '*') {
            int ns = new_state(), ne = new_state();
            add_edge(ns, s);
            add_edge(e, ne);
            add_edge(ns, ne);
            add_edge(e, s);
            return {ns, ne};
        } else if (q == '+') {
            int ns = new_state(), ne = new_state();
            add_edge(ns, s);
            add_edge(e, ne);
            add_edge(e, s);
            return {ns, ne};
        } else { // '?'
            int ns = new_state(), ne = new_state();
            add_edge(ns, s);
            add_edge(ns, ne);
            add_edge(e, ne);
            return {ns, ne};
        }
    }

    void build(const std::string& re) {
        std::size_t i = 0;
        auto [s, e] = build_fragment(re, i);

        // Handle top-level alternation: a|b
        // build_fragment stops at '|', so we need to handle it at the top level
        while (i < re.size() && re[i] == '|') {
            ++i;
            auto [s2, e2] = build_fragment(re, i);
            int ns = new_state(), ne = new_state();
            add_edge(ns, s);
            add_edge(ns, s2);
            add_edge(e, ne);
            add_edge(e2, ne);
            s = ns;
            e = ne;
        }

        // Connect state 0 to pattern start
        add_edge(0, s);
        accept_ = e;
    }
};

}  // namespace dsa
