template <typename T>
class FingerTree {
    struct Node2 {
        T a, b;
    };
    struct Node3 {
        T a, b, c;
    };

    enum class Tag { Empty, Single, Deep };

    struct Digit {
        std::vector<T> elems;
        int measure() const { return static_cast<int>(elems.size()); }
    };

    struct DeepData {
        int size;
        Digit prefix;
        FingerTree<Node2> middle;
        Digit suffix;
    };

    Tag tag_ = Tag::Empty;
    T single_;
    std::unique_ptr<DeepData> deep_;

public:
    FingerTree() = default;

    bool empty() const { return tag_ == Tag::Empty; }
    int size() const {
        if (tag_ == Tag::Empty)  return 0;
        if (tag_ == Tag::Single) return 1;
        return deep_->size;
    }

    void push_front(const T& val) {
        if (tag_ == Tag::Empty) {
            tag_ = Tag::Single;
            single_ = val;
            return;
        }
        if (tag_ == Tag::Single) {
            deep_ = std::make_unique<DeepData>();
            deep_->prefix.elems = {val};
            deep_->suffix.elems = {std::move(single_)};
            deep_->size = 2;
            tag_ = Tag::Deep;
            return;
        }
        if (deep_->prefix.elems.size() < 4) {
            deep_->prefix.elems.insert(
                deep_->prefix.elems.begin(), val);
            ++deep_->size;
        } else {
            Node2 node{val, deep_->prefix.elems[0]};
            deep_->prefix.elems.erase(
                deep_->prefix.elems.begin());
            deep_->middle.push_front(node);
            ++deep_->size;
        }
    }

    void push_back(const T& val) {
        if (tag_ == Tag::Empty) {
            tag_ = Tag::Single;
            single_ = val;
            return;
        }
        if (tag_ == Tag::Single) {
            deep_ = std::make_unique<DeepData>();
            deep_->prefix.elems = {std::move(single_)};
            deep_->suffix.elems = {val};
            deep_->size = 2;
            tag_ = Tag::Deep;
            return;
        }
        if (deep_->suffix.elems.size() < 4) {
            deep_->suffix.elems.push_back(val);
            ++deep_->size;
        } else {
            Node2 node{deep_->suffix.elems.back(), val};
            deep_->suffix.elems.pop_back();
            deep_->middle.push_back(node);
            ++deep_->size;
        }
    }
