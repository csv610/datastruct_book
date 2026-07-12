// Find best split among all features and thresholds
// Returns (best_feature, best_threshold, best_gain)
struct Split {
    int feature;
    double threshold;
    double gain;
};

Split find_best_split(const vector<vector<double>>& X,
                      const vector<int>& y) {
    int n = X.size();
    int d = X[0].size();
    Split best{-1, 0.0, -1.0};

    for (int f = 0; f < d; ++f) {
        // Collect unique values of feature f
        vector<double> vals;
        for (int i = 0; i < n; ++i) vals.push_back(X[i][f]);
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()),
                   vals.end());

        // Try each midpoint as threshold
        for (size_t i = 0; i + 1 < vals.size(); ++i) {
            double t = (vals[i] + vals[i + 1]) / 2.0;
            vector<int> left_y, right_y;
            for (int j = 0; j < n; ++j) {
                if (X[j][f] <= t) left_y.push_back(y[j]);
                else              right_y.push_back(y[j]);
            }
            double gain = information_gain(y, left_y, right_y);
            if (gain > best.gain)
                best = {f, t, gain};
        }
    }
    return best;
}

DecisionNode* build_tree(const vector<vector<double>>& X,
                         const vector<int>& y,
                         int depth, int max_depth) {
    // All same class => leaf
    if (all_same(y))
        return new DecisionNode(most_common(y));

    // Max depth or no split found => leaf
    if (depth >= max_depth)
        return new DecisionNode(most_common(y));

    Split s = find_best_split(X, y);
    if (s.gain <= 0)
        return new DecisionNode(most_common(y));

    vector<vector<double>> X_left, X_right;
    vector<int> y_left, y_right;
    for (size_t i = 0; i < X.size(); ++i) {
        if (X[i][s.feature] <= s.threshold) {
            X_left.push_back(X[i]);
            y_left.push_back(y[i]);
        } else {
            X_right.push_back(X[i]);
            y_right.push_back(y[i]);
        }
    }

    DecisionNode* left  = build_tree(X_left,  y_left,
                                      depth + 1, max_depth);
    DecisionNode* right = build_tree(X_right, y_right,
                                      depth + 1, max_depth);
    return new DecisionNode(s.feature, s.threshold,
                            left, right);
}
