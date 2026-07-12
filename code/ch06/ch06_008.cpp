std::stack<int> s;                          // uses std::deque by default
std::stack<int, std::vector<int>> sv;     // vector-based
std::stack<int, std::list<int>> sl;       // list-based

s.push(10);
s.push(20);
int x = s.top();  // 20
s.pop();          // removes 20
