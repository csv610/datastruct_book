class trie {
    struct node { node* children[26] = {}; bool is_end = false; };
    node root_;
public:
    void insert(std::string_view word) {
        node* cur = &root_;
        for (char c : word) {
            int i = c - 'a';
            if (!cur->children[i]) cur->children[i] = new node();
            cur = cur->children[i];
        }
        cur->is_end = true;
    }
    bool search(std::string_view word) {
        node* cur = &root_;
        for (char c : word) {
            int i = c - 'a';
            if (!cur->children[i]) return false;
            cur = cur->children[i];
        }
        return cur->is_end;
    }
    bool starts_with(std::string_view prefix) {
        node* cur = &root_;
        for (char c : prefix) {
            int i = c - 'a';
            if (!cur->children[i]) return false;
            cur = cur->children[i];
        }
        return true;
    }
};
