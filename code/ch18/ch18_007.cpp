procedure branch_and_bound(root):
    best = infinity
    queue = {root}
    while queue is not empty:
        node = queue.pop()
        if node is a complete solution:
            best = min(best, value(node))
        else if bound(node) < best:
            for each child of node:
                queue.push(child)
    return best
