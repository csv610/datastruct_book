struct DecisionNode {
    int feature_index;      // which feature to test
    double threshold;       // split threshold (numeric)
    int class_label;        // valid only at leaves
    bool is_leaf;
    DecisionNode* left;     // <= threshold  (or feature == false)
    DecisionNode* right;    // >  threshold  (or feature == true)

    // Leaf constructor
    DecisionNode(int label)
        : feature_index(-1), threshold(0),
          class_label(label), is_leaf(true),
          left(nullptr), right(nullptr) {}

    // Internal node constructor
    DecisionNode(int feat, double thresh,
                 DecisionNode* l, DecisionNode* r)
        : feature_index(feat), threshold(thresh),
          class_label(-1), is_leaf(false),
          left(l), right(r) {}
};
