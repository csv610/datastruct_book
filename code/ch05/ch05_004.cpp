// 2D array on stack (fixed size)
int matrix[3][4];

// 2D array on heap (C++ style)
auto matrix = std::make_unique<int[]>(rows * cols);

// Access: matrix[i * cols + j]

// Using vector of vectors
std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
