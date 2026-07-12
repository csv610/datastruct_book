class web_crawler {
public:
    web_crawler(std::string_view seed_url, size_t max_pages = 10000)
        : frontier_(), visited_(), max_pages_(max_pages) {
        frontier_.push(seed_url);
    }

    void crawl() {
        while (!frontier_.empty() && visited_.size() < max_pages_) {
            std::string url = frontier_.front(); frontier_.pop();
            if (visited_.count(url)) continue;
            visited_.insert(url);

            std::cout << "Crawling: " << url << " ("
                      << visited_.size() << "/" << max_pages_ << ")\n";

            auto links = fetch_links(url);  // HTTP GET + HTML parsing
            for (const auto& link : links) {
                if (!visited_.count(link) && same_domain(url, link)) {
                    frontier_.push(link);
                }
            }
        }
    }

    size_t pages_crawled() const { return visited_.size(); }

private:
    std::vector<std::string> fetch_links(std::string_view url) {
        // Simulate fetching -- in production this uses libcurl or CPPREST
        return {};
    }

    bool same_domain(std::string_view a, std::string_view b) {
        auto pos_a = a.find("://"), pos_b = b.find("://");
        if (pos_a == std::string_view::npos) pos_a = 0;
        else pos_a += 3;
        if (pos_b == std::string_view::npos) pos_b = 0;
        else pos_b += 3;
        std::string_view domain_a = a.substr(pos_a, a.find('/', pos_a) - pos_a);
        std::string_view domain_b = b.substr(pos_b, b.find('/', pos_b) - pos_b);
        return domain_a == domain_b;
    }

    linked_queue<std::string> frontier_;
    std::unordered_set<std::string> visited_;
    size_t max_pages_;
};
