std::deque<int> dq;
dq.push_back(10);     // [10]
dq.push_front(5);     // [5, 10]
dq.push_back(20);     // [5, 10, 20]
dq.pop_front();       // [10, 20]
dq.pop_back();        // [10]
