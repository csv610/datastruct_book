class consistent_hash_ring {
public:
    void add_server(std::string_view name, int virtual_nodes = 100) {
        for (int i = 0; i < virtual_nodes; ++i) {
            auto h = hash(std::string(name) + ":" + std::to_string(i));
            ring_[h] = name;
        }
    }

    void remove_server(std::string_view name) {
        std::erase_if(ring_, [&](const auto& p) {
            return p.second == name;
        });
    }

    std::string_view get_server(std::string_view key) const {
        if (ring_.empty()) return "";
        auto it = ring_.lower_bound(hash(key));
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }

private:
    static uint64_t hash(std::string_view s) {
        return std::hash<std::string_view>{}(s);
    }

    std::map<uint64_t, std::string> ring_;
};
