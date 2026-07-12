DFS from 0:
  Visit 0 -> visit 1 -> visit 3 (no unvisited neighbors)
    Push 3. Stack: [3]
  Back to 1: all neighbors done. Push 1. Stack: [3, 1]
  Back to 0: visit 2 -> 3 already visited. Push 2. Stack: [3, 1, 2]
  Back to 0: all done. Push 0. Stack: [3, 1, 2, 0]

Topological order (pop stack): 0, 2, 1, 3
Verification: 0->1 (0 before 1), 0->2 (0 before 2), 1->3 (1 before 3), 2->3 (2 before 3). Valid.
