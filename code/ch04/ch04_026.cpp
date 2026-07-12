template <typename T, int BLOCK_SIZE = 8>
class UnrolledList {
    struct Node {
        std::array<T, BLOCK_SIZE> data;
        int count = 0;
        Node* next = nullptr;
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    int size_ = 0;

public:
    UnrolledList() = default;
    ~UnrolledList() {
        Node* curr = head_;
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            for (int i = 0; i < tmp->count; ++i)
                tmp->data[i].~T();
            delete tmp;
        }
    }

    class iterator {
        Node* node_;
        int index_;
        friend class UnrolledList;
        iterator(Node* n, int i) : node_(n), index_(i) {}
    public:
        T& operator*()  { return node_->data[index_]; }
        T* operator->() { return &node_->data[index_]; }

        iterator& operator++() {
            ++index_;
            if (index_ >= node_->count) {
                node_ = node_->next;
                index_ = 0;
            }
            return *this;
        }

        bool operator!=(const iterator& o) const {
            return node_ != o.node_ || index_ != o.index_;
        }
    };

    iterator begin() { return iterator(head_, 0); }
    iterator end()   { return iterator(nullptr, 0); }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
