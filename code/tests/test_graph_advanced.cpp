#include "a_star.h"
#include "graph.h"
#include <cassert>
#include <print>

int main() {
  dsa::graph<int> g(5);
  g.add_directed_edge(0, 1, 2);
  g.add_directed_edge(1, 2, 2);
  g.add_directed_edge(0, 3, 1);
  g.add_directed_edge(3, 2, 10);
  g.add_directed_edge(2, 4, 1);
  auto result = dsa::a_star(g, 0, 4, [](std::size_t v, std::size_t) { return static_cast<int>(4 - v); });
  assert(result && result->distance == 5);
  assert((result->path == std::vector<std::size_t>{0, 1, 2, 4}));

  dsa::graph<int> undirected(5);
  undirected.add_edge(0, 1); undirected.add_edge(1, 2);
  undirected.add_edge(1, 3); undirected.add_edge(3, 4);
  const auto cuts = undirected.articulation_points();
  assert((cuts == std::vector<std::size_t>{1, 3}));
  const auto bridges = undirected.bridges();
  assert(bridges.size() == 4);
  const std::pair<std::size_t, std::size_t> first_bridge{0, 1};
  assert(bridges[0] == first_bridge);

  std::print("advanced graph algorithms: all passed\n");
}
