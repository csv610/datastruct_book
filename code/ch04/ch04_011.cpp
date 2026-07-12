struct layer {
    std::string name;
    bool visible = true;
    float opacity = 1.0f;
};

using layer_stack = dynamic_array_list<layer>;

void reorder_layer(layer_stack& stack, size_t from, size_t to) {
    layer l = std::move(stack[from]);
    stack.erase(from);
    stack.insert(to, std::move(l));
}
