// std::priority_queue is a max-heap by default.
std::priority_queue<int> max_pq;
max_pq.push(30);
max_pq.push(10);
max_pq.push(20);
std::cout << max_pq.top() << "\n";  // 30
max_pq.pop();
std::cout << max_pq.top() << "\n";  // 20

// Min-heap: use std::greater.
std::priority_queue<int, std::vector<int>,
                    std::greater<int>> min_pq;
min_pq.push(30);
min_pq.push(10);
min_pq.push(20);
std::cout << min_pq.top() << "\n";  // 10
