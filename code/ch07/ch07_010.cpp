template <typename T>
class deque_linked {
public:
    deque_linked() = default;

    ~deque_linked() {
        while (head_) {
            node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        node* n = new node(value);
        n->next = head_;
        if (head_) head_->prev = n;
        head_ = n;
        if (!tail_) tail_ = head_;
        ++sz_;
    }

    void push_back(const T& value) {
        node* n = new node(value);
        if (tail_) {
            tail_->next = n;
            n->prev = tail_;
        } else {
            head_ = n;
        }
        tail_ = n;
        ++sz_;
    }

    T pop_front() {
        if (!head_) throw std::underflow_error("empty");
        T val = head_->data;
        node* temp = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        delete temp;
        --sz_;
        return val;
    }

    T pop_back() {
        if (!tail_) throw std::underflow_error("empty");
        T val = tail_->data;
        node* temp = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;
        delete temp;
        --sz_;
        return val;
    }

    bool empty() const { return sz_ == 0; }
    int size() const { return sz_; }

private:
    struct node {
        T data;
        node* prev = nullptr;
        node* next = nullptr;
        explicit node(const T& d) : data(d) {}
    };
    node* head_ = nullptr;
    node* tail_ = nullptr;
    int sz_ = 0;
};
