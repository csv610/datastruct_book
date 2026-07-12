auto p = std::make_unique<int>(42);
auto s = std::make_shared<int>(42);
auto w = std::weak_ptr<int>(s);
