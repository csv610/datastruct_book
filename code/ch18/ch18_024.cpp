Backtracking          Dynamic Programming
Time (worst case)      O(2^n)                O(n * target)
Space                  O(n) recursion stack   O(n * target) table
Finds all solutions?   Yes (natural)          Needs reconstruction
Negative numbers?      Yes (with care)        Not directly
When pruning is good   Very fast (early exit) Same or faster
When pruning is poor   Slow (explores all)    Same or faster
