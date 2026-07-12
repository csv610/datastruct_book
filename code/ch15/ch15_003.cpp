struct item { double weight, value; };

double fractional_knapsack(double capacity, std::span<const item> items) {
    std::vector<size_t> order(items.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        return items[a].value / items[a].weight > items[b].value / items[b].weight;
    });

    double total_value = 0.0;
    for (size_t i : order) {
        if (capacity >= items[i].weight) {
            capacity -= items[i].weight;
            total_value += items[i].value;
        } else {
            total_value += items[i].value * (capacity / items[i].weight);
            break;
        }
    }
    return total_value;
}
