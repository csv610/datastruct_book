std::vector<int> v = {1, 2, 3, 4, 5};
v.push_back(6);          // {1, 2, 3, 4, 5, 6}
v.pop_back();            // {1, 2, 3, 4, 5}
int x = v[2];            // x = 3
v.insert(v.begin(), 0);  // {0, 1, 2, 3, 4, 5}
