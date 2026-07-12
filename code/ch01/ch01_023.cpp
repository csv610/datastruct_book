std::vector<int> v = {1, 2, 3};
for (int x : v) { /* read */ }
for (int& x : v) { /* modify */ }
for (const auto& x : v) { /* read, avoid copy */ }
