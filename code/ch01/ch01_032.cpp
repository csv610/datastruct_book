std::list<int> lst = {1, 3, 5};
auto it = lst.begin();
std::advance(it, 1);
lst.insert(it, 2);       // {1, 2, 3, 5}
lst.erase(it);           // {1, 2, 5}
