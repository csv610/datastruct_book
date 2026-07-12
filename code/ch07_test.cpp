#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <cassert>

// ch07: Queues - Circular queue, Deque, Priority Queue, BFS

// 1. Circular Queue
template <typename T>
class circular_queue {
    std::vector<T> data_;
    std::size_t head_ = 0, tail_ = 0, size_ = 0;
public:
    explicit circular_queue(std::size_t cap) : data_(cap + 1) {}
    
    void enqueue(const T& val) {
        if (full()) throw std::overflow_error("queue full");
        data_[tail_] = val;
        tail_ = (tail_ + 1) % data_.size();
        ++size_;
    }
    
    void dequeue() {
        if (empty()) throw std::underflow_error("queue empty");
        head_ = (head_ + 1) % data_.size();
        --size_;
    }
    
    T& front() { return data_[head_]; }
    const T& front() const { return data_[head_]; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == data_.size() - 1; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return data_.size() - 1; }
};

// 2. Deque using circular buffer
template <typename T>
class deque_circular {
    std::vector<T> data_;
    std::size_t head_ = 0, tail_ = 0, size_ = 0;
public:
    explicit deque_circular(std::size_t cap = 16) : data_(cap + 1) {}
    
    void push_front(const T& val) {
        if (full()) grow();
        head_ = (head_ + data_.size() - 1) % data_.size();
        data_[head_] = val;
        ++size_;
    }
    
    void push_back(const T& val) {
        if (full()) grow();
        data_[tail_] = val;
        tail_ = (tail_ + 1) % data_.size();
        ++size_;
    }
    
    void pop_front() {
        if (empty()) throw std::underflow_error("empty");
        head_ = (head_ + 1) % data_.size();
        --size_;
    }
    
    void pop_back() {
        if (empty()) throw std::underflow_error("empty");
        tail_ = (tail_ + data_.size() - 1) % data_.size();
        --size_;
    }
    
    T& front() { return data_[head_]; }
    T& back() { return data_[(tail_ + data_.size() - 1) % data_.size()]; }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    
private:
    bool full() const { return size_ == data_.size() - 1; }
    void grow() {
        std::vector<T> new_data(data_.size() * 2);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[(head_ + i) % data_.size()]);
        }
        data_ = std::move(new_data);
        head_ = 0;
        tail_ = size_;
    }
};

// 3. Sliding Window Maximum
std::vector<int> sliding_window_max(const std::vector<int>& nums, int k) {
    std::deque<int> dq;  // stores indices
    std::vector<int> result;
    
    for (int i = 0; i < (int)nums.size(); ++i) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}

// 4. Zigzag Level Order Traversal
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

std::vector<std::vector<int>> zigzag_level_order(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (!root) return result;
    std::queue<TreeNode*> q;
    q.push(root);
    bool left_to_right = true;
    
    while (!q.empty()) {
        int sz = q.size();
        std::vector<int> level(sz);
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            int idx = left_to_right ? i : sz - 1 - i;
            level[idx] = node->val;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
        left_to_right = !left_to_right;
    }
    return result;
}

// 5. Right Side View
std::vector<int> right_side_view(TreeNode* root) {
    std::vector<int> result;
    if (!root) return result;
    std::queue<std::pair<TreeNode*, int>> q;
    q.push({root, 0});
    
    while (!q.empty()) {
        auto [node, depth] = q.front(); q.pop();
        if (depth == (int)result.size()) result.push_back(node->val);
        if (node->right) q.push({node->right, depth + 1});
        if (node->left) q.push({node->left, depth + 1});
    }
    return result;
}

// 6. Priority Queue for Dijkstra
struct Edge { int to, weight; };
using Graph = std::vector<std::vector<Edge>>;

std::vector<int> dijkstra(const Graph& g, int src) {
    std::vector<int> dist(g.size(), INT_MAX);
    dist[src] = 0;
    using P = std::pair<int, int>;  // {dist, node}
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// 6. Word Ladder (BFS)
int word_ladder_length(const std::string& begin, const std::string& end, 
                       const std::vector<std::string>& word_list) {
    std::unordered_set<std::string> dict(word_list.begin(), word_list.end());
    if (!dict.count(end)) return 0;
    
    std::queue<std::pair<std::string, int>> q;
    q.push({begin, 1});
    std::unordered_set<std::string> visited{begin};
    
    while (!q.empty()) {
        auto [word, len] = q.front(); q.pop();
        if (word == end) return len;
        
        for (int i = 0; i < (int)word.size(); ++i) {
            char original = word[i];
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == original) continue;
                std::string next = word;
                next[i] = c;
                if (dict.count(next) && !visited.count(next)) {
                    visited.insert(next);
                    q.push({next, len + 1});
                }
            }
        }
    }
    return 0;
}

// 7. Bipartite Check
bool is_bipartite(const Graph& g) {
    std::vector<int> color(g.size(), -1);
    for (int s = 0; s < (int)g.size(); ++s) {
        if (color[s] != -1) continue;
        std::queue<int> q;
        q.push(s);
        color[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, _] : g[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void test_circular_queue() {
    std::cout << "=== Testing Circular Queue ===\n";
    circular_queue<int> cq(5);
    for (int i = 0; i < 5; ++i) cq.enqueue(i);
    assert(cq.size() == 5);
    assert(cq.full());
    cq.dequeue();
    cq.enqueue(5);
    assert(cq.front() == 1);
    assert(cq.size() == 5);
    std::cout << "Circular Queue: PASS\n";
}

void test_deque_circular() {
    std::cout << "=== Testing Circular Deque ===\n";
    deque_circular<int> dq(4);
    dq.push_back(1); dq.push_back(2);
    dq.push_front(0); dq.push_front(-1);
    assert(dq.size() == 4);
    assert(dq.front() == -1);
    assert(dq.back() == 2);
    dq.pop_front(); dq.pop_back();
    assert(dq.front() == 0);
    assert(dq.back() == 1);
    std::cout << "Circular Deque: PASS\n";
}

void test_sliding_window() {
    std::cout << "=== Testing Sliding Window Max ===\n";
    std::vector<int> nums = {1,3,-1,-3,5,3,6,7};
    auto result = sliding_window_max(nums, 3);
    assert(result == std::vector<int>({3,3,5,5,6,7}));
    std::cout << "Sliding Window Max: PASS\n";
}

void test_zigzag() {
    std::cout << "=== Testing Zigzag Level Order ===\n";
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    auto result = zigzag_level_order(root);
    assert(result == std::vector<std::vector<int>>{{3}, {20,9}, {15,7}});
    std::cout << "Zigzag Level Order: PASS\n";
}

void test_right_side_view() {
    std::cout << "=== Testing Right Side View ===\n";
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(4);
    auto result = right_side_view(root);
    assert(result == std::vector<int>({1,3,4}));
    std::cout << "Right Side View: PASS\n";
}

void test_dijkstra() {
    std::cout << "=== Testing Dijkstra ===\n";
    Graph g(5);
    g[0].push_back({1, 4}); g[0].push_back({2, 1});
    g[1].push_back({3, 1});
    g[2].push_back({1, 2}); g[2].push_back({3, 5});
    g[3].push_back({4, 3});
    auto dist = dijkstra(g, 0);
    assert(dist == std::vector<int>({0, 3, 1, 4, 7}));
    std::cout << "Dijkstra: PASS\n";
}

void test_word_ladder() {
    std::cout << "=== Testing Word Ladder ===\n";
    std::vector<std::string> dict = {"hot","dot","dog","lot","log","cog"};
    assert(word_ladder_length("hit", "cog", dict) == 5);
    assert(word_ladder_length("hit", "cog", {"hot","dot","dog","lot","log"}) == 0);
    std::cout << "Word Ladder: PASS\n";
}

void test_bipartite() {
    std::cout << "=== Testing Bipartite Check ===\n";
    Graph g1(4);
    g1[0].push_back({1,0}); g1[1].push_back({0,0}); g1[1].push_back({2,0});
    g1[2].push_back({1,0}); g1[2].push_back({3,0}); g1[3].push_back({2,0});
    assert(is_bipartite(g1) == true);
    
    Graph g2(3);
    g2[0].push_back({1,0}); g2[1].push_back({0,0}); g2[1].push_back({2,0});
    g2[2].push_back({1,0}); g2[2].push_back({0,0}); g2[0].push_back({2,0});
    assert(is_bipartite(g2) == false);
    std::cout << "Bipartite Check: PASS\n";
}

int main() {
    std::cout << "=== Chapter 7: Queues ===\n\n";
    test_circular_queue();
    test_deque_circular();
    test_sliding_window();
    test_zigzag();
    test_right_side_view();
    test_dijkstra();
    test_word_ladder();
    test_bipartite();
    std::cout << "\n=== All tests passed ===\n";
    return 0;
}