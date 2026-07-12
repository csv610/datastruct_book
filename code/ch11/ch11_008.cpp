// Simplified B+ tree internal node concept:
struct btree_node {
    std::vector<int> keys;           // separating keys
    std::vector<btree_node*> children; // child pointers
    bool is_leaf;
};
