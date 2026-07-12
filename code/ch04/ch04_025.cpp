std::forward_list<int> fl = {1, 2, 3};
fl.push_front(0);
auto it = fl.begin(); ++it;
fl.insert_after(it, 5);                 // {0, 1, 5, 2, 3}
fl.erase_after(it);                     // {0, 1, 2, 3}

std::list<int> l = {1, 2, 3};
l.push_front(0);
l.push_back(4);
l.pop_front();
l.pop_back();
