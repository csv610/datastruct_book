#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

using Entry = pair<int, string>;   // (count, element)

// Min-heap of size k: keep the k elements with the largest counts.
class TopK {
public:
    TopK(size_t k) : k_(k) {}

    void insert(const string& item) {
        int& cnt = freq_[item];
        cnt++;
        if (in_heap_.count(item)) {
            // Already in heap; will be rebuilt on next topK().
            stale_ = true;
            return;
        }
        if (heap_.size() < k_) {
            heap_.emplace(cnt, item);
            in_heap_.insert(item);
        } else if (cnt > heap_.top().first) {
            auto [old_cnt, old_item] = heap_.top();
            heap_.pop();
            in_heap_.erase(old_item);
            heap_.emplace(cnt, item);
            in_heap_.insert(item);
        }
    }

    vector<string> topK() {
        if (stale_) rebuild();
        vector<string> result;
        while (!heap_.empty()) {
            result.push_back(heap_.top().second);
            heap_.pop();
        }
        reverse(result.begin(), result.end());
        in_heap_.clear();
        stale_ = false;
        return result;
    }

private:
    void rebuild() {
        // Rebuild heap from current frequency map.
        priority_queue<Entry, vector<Entry>, greater<>> fresh;
        for (auto& [item, cnt] : freq_) {
            if (fresh.size() < k_) {
                fresh.emplace(cnt, item);
            } else if (cnt > fresh.top().first) {
                fresh.pop();
                fresh.emplace(cnt, item);
            }
        }
        heap_ = move(fresh);
    }

    size_t k_;
    unordered_map<string, int> freq_;
    priority_queue<Entry, vector<Entry>, greater<>> heap_;
    unordered_set<string> in_heap_;
    bool stale_ = false;
};
