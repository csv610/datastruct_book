#include "graph.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
  dsa::graph<int> g(5);
  g.add_directed_edge(0, 1, 1);
  g.add_directed_edge(0, 2, 4);
  g.add_directed_edge(1, 2, 2);
  g.add_directed_edge(1, 3, 6);
  g.add_directed_edge(2, 3, 1);
  g.add_directed_edge(3, 4, 3);

  auto dist = g.bellman_ford(0);
  assert(dist.size() == 5);
  assert(dist[0] == 0);
  assert(dist[1] == 1);
  assert(dist[2] == 3);
  assert(dist[3] == 4);
  assert(dist[4] == 7);

  dsa::graph<int> neg(4);
  neg.add_directed_edge(0, 1, 1);
  neg.add_directed_edge(1, 2, -3);
  neg.add_directed_edge(2, 0, 1);
  assert(neg.bellman_ford(0).empty());

  dsa::graph<int> fw(3);
  fw.add_directed_edge(0, 1, 3);
  fw.add_directed_edge(0, 2, 8);
  fw.add_directed_edge(1, 2, -2);
  auto result = fw.floyd_warshall();
  assert(result.size() == 3);
  assert(result[0][0] == 0);
  assert(result[0][1] == 3);
  assert(result[0][2] == 1);
  assert(result[1][1] == 0);
  assert(result[1][2] == -2);
  assert(result[2][2] == 0);

  dsa::graph<int> ac(4);
  ac.add_directed_edge(0, 1);
  ac.add_directed_edge(1, 2);
  ac.add_directed_edge(2, 3);
  assert(!ac.has_cycle());

  dsa::graph<int> cc(3);
  cc.add_directed_edge(0, 1);
  cc.add_directed_edge(1, 2);
  cc.add_directed_edge(2, 0);
  assert(cc.has_cycle());

  dsa::graph<int> self(1);
  self.add_directed_edge(0, 0);
  assert(self.has_cycle());

  dsa::graph<int> undir(3);
  undir.add_edge(0, 1);
  undir.add_edge(1, 2);
  auto comp = undir.connected_components();
  assert(comp[0] == comp[1]);
  assert(comp[1] == comp[2]);

  auto bfs_d = undir.bfs_distances(0);
  assert(bfs_d[0] == 0);
  assert(bfs_d[1] == 1);
  assert(bfs_d[2] == 2);

  std::vector<int> visited;
  undir.dfs(0, [&](std::size_t v) { visited.push_back(static_cast<int>(v)); });
  assert(visited.size() == 3);

  visited.clear();
  undir.bfs(0, [&](std::size_t v) { visited.push_back(static_cast<int>(v)); });
  assert(visited[0] == 0);
  assert(visited.size() == 3);

  dsa::graph<int> topo(4);
  topo.add_directed_edge(0, 1);
  topo.add_directed_edge(0, 2);
  topo.add_directed_edge(1, 3);
  topo.add_directed_edge(2, 3);
  auto order = topo.topological_sort();
  assert(order.size() == 4);

  auto mst = dsa::kruskal_mst(4, {{0,1,1},{0,2,4},{1,2,2},{2,3,3}});
  assert(mst.size() == 3);

  std::print("graph: all passed\n");
}
