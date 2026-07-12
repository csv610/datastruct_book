std::queue<int> q;                          // uses std::deque
std::queue<int, std::list<int>> ql;       // list-based

q.push(10);
q.push(20);
int x = q.front();  // 10
q.pop();            // removes 10
