static FingerTree concat(FingerTree left, FingerTree right) {
        if (left.empty()) return right;
        if (right.empty()) return left;
        if (left.tag_ == Tag::Single)
            { left.push_back(right.single_); return left; }
        if (right.tag_ == Tag::Single)
            { left.push_back(right.single_); return left; }
        // both are Deep: fold right tree's prefix
        // and suffix through the middle
        FingerTree result;
        result.tag_ = Tag::Deep;
        result.deep_ = std::make_unique<DeepData>();
        result.deep_->prefix = left.deep_->prefix;
        result.deep_->suffix = right.deep_->suffix;
        result.deep_->middle = FingerTree::concat_trees(
            left.deep_->middle,
            left.deep_->suffix,
            right.deep_->prefix,
            right.deep_->middle);
        result.deep_->size = left.size() + right.size();
        return result;
    }

    std::pair<FingerTree, FingerTree> split(int idx) {
        if (tag_ == Tag::Empty)
            return {FingerTree(), FingerTree()};
        if (tag_ == Tag::Single) {
            if (idx <= 0)
                return {FingerTree(), *this};
            return {*this, FingerTree()};
        }
        int psize = deep_->prefix.measure();
        if (idx <= psize) {
            // split within prefix
            FingerTree left, right;
            left.tag_ = Tag::Empty;
            right.tag_ = Tag::Deep;
            // ... partition prefix at idx
            return {left, right};
        }
        // split within middle or suffix
        // (recursive decomposition)
        return {FingerTree(), *this};
    }
