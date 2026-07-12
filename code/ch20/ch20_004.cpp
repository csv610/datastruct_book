class push_relabel {
public:
    push_relabel(size_t n) : graph_(n), excess_(n), height_(n) {}

    int max_flow(size_t s, size_t t) {
        height_[s] = graph_.size();
        for (auto& e : graph_[s]) {
            excess_[e.to] += e.capacity;
            graph_[e.to][e.rev].capacity = e.capacity;
            e.capacity = 0;
        }
        while (true) {
            auto it = std::find_if(excess_.begin(), excess_.end(),
                [&](int x) { return x > 0; });
            if (it == excess_.end()) break;
            size_t v = it - excess_.begin();
            if (!discharge(v)) break;
        }
        return excess_[t];
    }

private:
    bool push(size_t v) {
        for (auto& e : graph_[v]) {
            if (e.capacity > 0 && height_[v] == height_[e.to] + 1) {
                int d = std::min(excess_[v], e.capacity);
                e.capacity -= d;
                graph_[e.to][e.rev].capacity += d;
                excess_[v] -= d;
                excess_[e.to] += d;
                return true;
            }
        }
        return false;
    }

    bool discharge(size_t v) {
        while (excess_[v] > 0) {
            if (!push(v)) {
                int min_h = INT_MAX;
                for (const auto& e : graph_[v])
                    if (e.capacity > 0)
                        min_h = std::min(min_h, height_[e.to]);
                if (min_h >= static_cast<int>(graph_.size()))
                    return false;
                height_[v] = min_h + 1;
            }
        }
        return true;
    }

    std::vector<std::vector<edge>> graph_;
    std::vector<int> excess_, height_;
};
