std::priority_queue<int> pq;  // max-heap
pq.push(30);
pq.push(10);
pq.push(50);
std::cout << pq.top();  // 50
pq.pop();               // removes 50
std::cout << pq.top();  // 30
