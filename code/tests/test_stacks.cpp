#include "stacks.h"
#include <cassert>
#include <iostream>
#include <string>

int main() {
  dsa::array_stack<int> as;
  assert(as.empty());
  as.push(1); as.push(2); as.push(3);
  assert(as.size() == 3);
  assert(as.top() == 3);
  as.pop();
  assert(as.top() == 2);

  dsa::linked_stack<int> ls;
  ls.push(10); ls.push(20);
  assert(ls.top() == 20);
  ls.pop();
  assert(ls.top() == 10);
  ls.pop();
  assert(ls.empty());

  dsa::stack<int> s;
  s.push(100); s.push(200);
  assert(s.top() == 200);
  s.pop();
  assert(s.top() == 100);

  assert(dsa::is_balanced("([{}])"));
  assert(dsa::is_balanced(""));
  assert(!dsa::is_balanced("([)]"));
  assert(!dsa::is_balanced("("));

  assert(dsa::evaluate_postfix("3 4 + 2 *") == 14);
  assert(dsa::evaluate_postfix("5 1 2 + 4 * + 3 -") == 14);

  assert(dsa::infix_to_postfix("3+4*2") == "342*+");
  assert(dsa::infix_to_postfix("(3+4)*2") == "34+2*");

  std::cout << "stacks: all passed\n";
}
