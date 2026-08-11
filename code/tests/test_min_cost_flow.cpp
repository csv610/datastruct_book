#include "min_cost_flow.h"
#include <cassert>
#include <print>

int main() {
  dsa::min_cost_max_flow<int, long long> flow(4);
  flow.add_edge(0, 1, 2, 1);
  flow.add_edge(0, 2, 1, 5);
  flow.add_edge(1, 2, 1, 1);
  flow.add_edge(1, 3, 1, 3);
  flow.add_edge(2, 3, 2, 1);
  const auto answer = flow.solve(0, 3);
  assert(answer.flow == 3 && answer.cost == 13);

  dsa::min_cost_max_flow<int, long long> negative(3);
  negative.add_edge(0, 1, 1, -2);
  negative.add_edge(1, 2, 1, 3);
  assert(negative.solve(0, 2).cost == 1);
  std::print("min-cost max-flow: all passed\n");
}
