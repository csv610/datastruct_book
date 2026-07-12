void insert(int pos, const T& value) {
        if (!head_ || pos == 0) {
            push_front(value);
            return;
        }
        Node* prev = nullptr;
        Node* curr = head_;
        int offset = pos;
        while (curr && offset > curr->count) {
            offset -= curr->count;
            prev = curr;
            curr = curr->next;
        }
        if (!curr) {
            append_to_node(tail_, value);
            return;
        }
        if (curr->count < BLOCK_SIZE) {
            shift_right(curr, offset);
            curr->data[offset] = value;
            ++curr->count;
        } else {
            Node* new_node = new Node();
            split_node(curr, new_node);
            if (offset <= curr->count) {
                shift_right(curr, offset);
                curr->data[offset] = value;
                ++curr->count;
            } else {
                shift_right(new_node, offset - curr->count);
                new_node->data[offset - curr->count] = value;
                ++new_node->count;
            }
        }
        ++size_;
    }

    void erase(int pos) {
        Node* prev = nullptr;
        Node* curr = head_;
        int offset = pos;
        while (offset >= curr->count) {
            offset -= curr->count;
            prev = curr;
            curr = curr->next;
        }
        shift_left(curr, offset);
        --curr->count;
        --size_;
        if (curr->count == 0 && prev) {
            prev->next = curr->next;
            if (curr == tail_) tail_ = prev;
            delete curr;
        }
    }

private:
    void shift_right(Node* n, int from) {
        for (int i = n->count; i > from; --i)
            n->data[i] = std::move(n->data[i - 1]);
    }
    void shift_left(Node* n, int from) {
        for (int i = from; i < n->count - 1; ++i)
            n->data[i] = std::move(n->data[i + 1]);
    }
    void split_node(Node* old_node, Node* new_node) {
        int half = old_node->count / 2;
        for (int i = half; i < old_node->count; ++i)
            new_node->data[i - half] = std::move(old_node->data[i]);
        new_node->count = old_node->count - half;
        old_node->count = half;
        new_node->next = old_node->next;
        old_node->next = new_node;
    }
    void push_front(const T& value) { /* allocate node, insert */ }
    void append_to_node(Node* n, const T& value) { /* insert at end */ }
