struct huffman_node {
    char ch;
    int freq;
    huffman_node* left;
    huffman_node* right;
};

struct compare {
    bool operator()(huffman_node* a, huffman_node* b) {
        return a->freq > b->freq;
    }
};

huffman_node* build_huffman_tree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> pq;

    for (const auto& [ch, freq] : frequencies) {
        pq.push(new huffman_node{ch, freq, nullptr, nullptr});
    }

    while (pq.size() > 1) {
        auto* left = pq.top(); pq.pop();
        auto* right = pq.top(); pq.pop();
        auto* parent = new huffman_node{'\0', left->freq + right->freq, left, right};
        pq.push(parent);
    }
    return pq.top();
}

void build_codes(huffman_node* root, std::string prefix,
                 std::unordered_map<char, std::string>& codes) {
    if (!root->left && !root->right) {
        codes[root->ch] = prefix;
        return;
    }
    build_codes(root->left, prefix + "0", codes);
    build_codes(root->right, prefix + "1", codes);
}
