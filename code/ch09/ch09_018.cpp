struct ThreadedNode {
    int data;
    ThreadedNode* left;
    ThreadedNode* right;
    bool left_thread;   // true => left is a thread
    bool right_thread;  // true => right is a thread

    ThreadedNode(int val)
        : data(val), left(nullptr), right(nullptr),
          left_thread(false), right_thread(false) {}
};

// Insert a node as the inorder successor of 'parent'
ThreadedNode* insert_right(ThreadedNode* parent,
                           ThreadedNode* child) {
    child->right = parent->right;
    child->right_thread = parent->right_thread;
    child->left = parent;
    child->left_thread = true;
    parent->right = child;
    parent->right_thread = false;
    if (child->right_thread)
        child->right->left = child;
    return child;
}
