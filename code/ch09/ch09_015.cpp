int predict(DecisionNode* root,
            const vector<double>& sample) {
    DecisionNode* curr = root;
    while (!curr->is_leaf) {
        if (sample[curr->feature_index] <= curr->threshold)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return curr->class_label;
}
