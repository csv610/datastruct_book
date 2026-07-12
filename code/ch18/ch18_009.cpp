struct puzzle_state {
    std::vector<int> board;  // 16 elements, 0 = empty
    int zero_pos;
    int g;  // cost so far (moves made)
    int h;  // heuristic (Manhattan distance)
    
    int f() const { return g + h; }
    
    bool operator>(const puzzle_state& other) const {
        return f() > other.f();
    }
};

int manhattan_distance(const std::vector<int>& board) {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] == 0) continue;
        int target_row = (board[i] - 1) / 4;
        int target_col = (board[i] - 1) % 4;
        int row = i / 4;
        int col = i % 4;
        distance += std::abs(row - target_row) + std::abs(col - target_col);
    }
    return distance;
}

// A* search is branch and bound with a priority queue ordered by f = g + h
int solve_15_puzzle(const std::vector<int>& start) {
    std::priority_queue<puzzle_state, std::vector<puzzle_state>,
                        std::greater<puzzle_state>> pq;
    
    int start_zero = std::find(start.begin(), start.end(), 0) - start.begin();
    pq.push({start, start_zero, 0, manhattan_distance(start)});
    
    std::set<std::vector<int>> visited;
    
    while (!pq.empty()) {
        auto state = pq.top(); pq.pop();
        if (state.h == 0) return state.g;  // solved
        
        if (visited.count(state.board)) continue;
        visited.insert(state.board);
        
        // Generate moves (up, down, left, right)
        int row = state.zero_pos / 4;
        int col = state.zero_pos % 4;
        int dirs[][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        for (auto [dr, dc] : dirs) {
            int nr = row + dr, nc = col + dc;
            if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4) continue;
            
            auto new_board = state.board;
            int new_pos = nr * 4 + nc;
            std::swap(new_board[state.zero_pos], new_board[new_pos]);
            
            if (!visited.count(new_board)) {
                pq.push({new_board, new_pos, state.g + 1,
                         manhattan_distance(new_board)});
            }
        }
    }
    return -1;  // unsolvable
}
