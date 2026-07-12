# Arrays, Matrices, and Sparse Representations

## Introduction

In Chapter 4 we used one-dimensional arrays to represent linear lists. In this chapter we generalize to multi-dimensional arrays and matrices, including techniques for representing **special matrices** (triangular, diagonal) and **sparse matrices** where most elements are zero.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Multi-Dimensional Arrays

### Row-Major and Column-Major Ordering

A 2D array with r rows and c columns is stored linearly in memory. Two standard mappings exist:

**Row-major order** (used by C++): elements of row 0, then row 1, etc.
```
Index(i, j) = i * c + j
```

**Column-major order** (used by Fortran): elements of column 0, then column 1, etc.
```
Index(i, j) = j * r + i
```

C++ uses row-major order. For a 3×4 matrix:

```
a[0][0] a[0][1] a[0][2] a[0][3] a[1][0] a[1][1] ...
  0       1       2       3       4       5
```

### Multi-Dimensional Arrays in C++

```cpp
// 2D array on stack (fixed size)
int matrix[3][4];

// 2D array on heap (C++ style)
auto matrix = std::make_unique<int[]>(rows * cols);

// Access: matrix[i * cols + j]

// Using vector of vectors
std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
```

### A Generic Matrix Class

```cpp
template <typename T>
class matrix {
public:
    matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(std::make_unique<T[]>(rows * cols)) {}

    T& operator()(size_t i, size_t j) {
        return data_[i * cols_ + j];
    }
    const T& operator()(size_t i, size_t j) const {
        return data_[i * cols_ + j];
    }

    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }
    size_t size() const noexcept { return rows_ * cols_; }

    matrix operator+(const matrix& other) const {
        matrix result(rows_, cols_);
        for (size_t k = 0; k < size(); ++k) {
            result.data_[k] = data_[k] + other.data_[k];
        }
        return result;
    }

    matrix operator*(const matrix& other) const {
        matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t k = 0; k < cols_; ++k) {
                T aik = (*this)(i, k);
                if (aik != T(0)) {  // skip zero multiplies
                    for (size_t j = 0; j < other.cols_; ++j) {
                        result(i, j) += aik * other(k, j);
                    }
                }
            }
        }
        return result;
    }

private:
    size_t rows_, cols_;
    std::unique_ptr<T[]> data_;
};
```

## Special Matrices

Many problems involve matrices with a known structure. We can save space by storing only the unique elements.

### Diagonal Matrix

A diagonal matrix has non-zero entries only on the main diagonal:

```
[d₀  0   0 ]
[ 0  d₁  0 ]
[ 0   0  d₂]
```

Store as a 1D array of size n: `d[i]` stores the element at (i, i).

```cpp
template <typename T>
class diagonal_matrix {
public:
    explicit diagonal_matrix(size_t n) : n_(n), data_(std::make_unique<T[]>(n)) {}

    T& operator()(size_t i, size_t j) {
        if (i != j) throw std::logic_error("non-diagonal access");
        return data_[i];
    }
    const T& operator()(size_t i, size_t j) const {
        if (i != j) return zero_;
        return data_[i];
    }

private:
    size_t n_;
    std::unique_ptr<T[]> data_;
    static constexpr T zero_ = T(0);
};
```

### Triangular Matrices

Upper triangular: all entries below the main diagonal are zero.
Lower triangular: all entries above the main diagonal are zero.

```
Lower triangular (n=4):
[a₀₀  0    0    0  ]
[a₁₀  a₁₁  0    0  ]
[a₂₀  a₂₁  a₂₂  0  ]
[a₃₀  a₃₁  a₃₂  a₃₃]
```

Store in a 1D array of size n(n+1)/2:

```
Row-major mapping for lower triangular: index(i, j) = i(i+1)/2 + j   for i ≥ j
```

```cpp
template <typename T>
class lower_triangular_matrix {
public:
    explicit lower_triangular_matrix(size_t n)
        : n_(n), data_(std::make_unique<T[]>(n * (n + 1) / 2)) {}

    T& operator()(size_t i, size_t j) {
        if (i < j) throw std::logic_error("below diagonal");
        return data_[i * (i + 1) / 2 + j];
    }
    const T& operator()(size_t i, size_t j) const {
        if (i < j) return zero_;
        return data_[i * (i + 1) / 2 + j];
    }

private:
    size_t n_;
    std::unique_ptr<T[]> data_;
    static constexpr T zero_ = T(0);
};
```

**Space comparison for n=1000:**
- Full array: 1,000,000 elements
- Triangular: 500,500 elements — ~50% savings

### Tridiagonal Matrix

Non-zero only on the main diagonal, first super-diagonal, and first sub-diagonal:

```
[a₀  b₀  0   0   0 ]
[c₀  a₁  b₁  0   0 ]
[ 0  c₁  a₂  b₂  0 ]
[ 0   0  c₂  a₃  b₃]
[ 0   0   0  c₃  a₄]
```

Store in three 1D arrays (a, b, c) or interleaved: 3n-2 elements.

## Sparse Matrices

When a matrix has few non-zero elements (e.g., <5%), storing all zeros is wasteful. We instead store only the non-zero entries, each with its position.

### Coordinate Storage (COO)

Store a list of (row, col, value) triples:

```cpp
struct triple {
    size_t row;
    size_t col;
    double value;
};

class sparse_matrix_coo {
public:
    sparse_matrix_coo(size_t rows, size_t cols)
        : rows_(rows), cols_(cols) {}

    void add_entry(size_t row, size_t col, double value) {
        if (value != 0.0) {
            entries_.push_back({row, col, value});
        }
    }

    double operator()(size_t row, size_t col) const {
        for (const auto& e : entries_) {
            if (e.row == row && e.col == col) return e.value;
        }
        return 0.0;
    }

    size_t nonzero_count() const noexcept { return entries_.size(); }
    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }

private:
    size_t rows_, cols_;
    std::vector<triple> entries_;  // unsorted; sort by (row, col) for efficiency
};
```

### Compressed Sparse Row (CSR)

CSR is the most widely used sparse format for linear algebra. It stores:
- **values**: non-zero values, in row-major order
- **col_indices**: column index of each value
- **row_ptr**: start index of each row in values/col_indices

```
Matrix:
[1 0 2]
[0 3 0]
[4 0 5]

values:      [1, 2, 3, 4, 5]
col_indices: [0, 2, 1, 0, 2]
row_ptr:     [0, 2, 3, 5]
```

```cpp
template <typename T>
class csr_matrix {
public:
    csr_matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), row_ptr_(rows + 1, 0) {}

    void finalize() {
        // Convert from COO to CSR
        std::sort(coo_.begin(), coo_.end(),
            [](const auto& a, const auto& b) {
                return std::tie(a.row, a.col) < std::tie(b.row, b.col);
            });

        // Build CSR
        size_t nnz = coo_.size();
        values_.reserve(nnz);
        col_indices_.reserve(nnz);

        for (const auto& t : coo_) {
            values_.push_back(t.value);
            col_indices_.push_back(t.col);
        }

        // Build row_ptr
        size_t idx = 0;
        for (size_t i = 0; i < rows_; ++i) {
            row_ptr_[i] = idx;
            while (idx < nnz && coo_[idx].row == i) ++idx;
        }
        row_ptr_[rows_] = nnz;

        coo_.clear();  // free temporary storage
    }

    T operator()(size_t row, size_t col) const {
        for (size_t i = row_ptr_[row]; i < row_ptr_[row + 1]; ++i) {
            if (col_indices_[i] == col) return values_[i];
        }
        return T(0);
    }

    // Sparse matrix-vector multiplication: result = A * x
    std::vector<T> multiply(const std::vector<T>& x) const {
        std::vector<T> result(rows_, T(0));
        for (size_t i = 0; i < rows_; ++i) {
            T sum = T(0);
            for (size_t j = row_ptr_[i]; j < row_ptr_[i + 1]; ++j) {
                sum += values_[j] * x[col_indices_[j]];
            }
            result[i] = sum;
        }
        return result;
    }

    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }
    size_t nonzero_count() const noexcept { return values_.size(); }

private:
    size_t rows_, cols_;
    std::vector<T> values_;
    std::vector<size_t> col_indices_;
    std::vector<size_t> row_ptr_;
    std::vector<triple> coo_;  // temporary during construction
};
```

### Complexity Comparison

| Operation | Dense O(n²) | CSR Sparse |
|-----------|-------------|------------|
| Access A(i,j) | O(1) | O(k) where k = nonzeros per row |
| Matrix-vector multiply | O(n²) | O(nnz) |
| Memory | O(n²) | O(n + nnz) |

**Example:** A 10,000 × 10,000 matrix with 50,000 non-zeros (0.05% density)
- Dense: 800 MB (double precision)
- CSR: ~1.2 MB (values + indices + row_ptr)

## Application: PageRank

The PageRank algorithm computes the importance of web pages using sparse matrix-vector multiplication. The web graph is represented as a sparse matrix where A(i,j) = 1 if page j links to page i.

```cpp
std::vector<double> pagerank(const csr_matrix<double>& web_graph,
                              size_t iterations = 100,
                              double damping = 0.85) {
    size_t n = web_graph.rows();
    std::vector<double> rank(n, 1.0 / n);
    std::vector<double> new_rank(n, 0.0);
    double teleport = (1.0 - damping) / n;

    for (size_t iter = 0; iter < iterations; ++iter) {
        // Sparse matrix-vector multiply
        new_rank = web_graph.multiply(rank);
        
        // Apply damping and teleportation
        double sum = 0.0;
        for (size_t i = 0; i < n; ++i) {
            new_rank[i] = damping * new_rank[i] + teleport;
            sum += new_rank[i];
        }
        
        // Normalize
        for (size_t i = 0; i < n; ++i) new_rank[i] /= sum;
        
        std::swap(rank, new_rank);
    }
    return rank;
}
```

## Application: Image Processing

Images are 2D arrays (or 3D for color). Matrix operations form the foundation of image processing:

```cpp
// Grayscale image as a matrix
using image = matrix<uint8_t>;

// Convolution: apply a kernel to every pixel
image apply_filter(const image& img, const matrix<double>& kernel) {
    image result(img.rows(), img.cols());
    int k_half = kernel.rows() / 2;

    for (size_t i = k_half; i < img.rows() - k_half; ++i) {
        for (size_t j = k_half; j < img.cols() - k_half; ++j) {
            double sum = 0.0;
            for (size_t ki = 0; ki < kernel.rows(); ++ki) {
                for (size_t kj = 0; kj < kernel.cols(); ++kj) {
                    sum += kernel(ki, kj) * img(i + ki - k_half, j + kj - k_half);
                }
            }
            result(i, j) = static_cast<uint8_t>(std::clamp(sum, 0.0, 255.0));
        }
    }
    return result;
}
```

## STL Connection: std::mdspan (C++23)

C++23 introduces `std::mdspan` — a non-owning multi-dimensional array view:

```cpp
#include <mdspan>
#include <vector>

std::vector<int> data = {1, 2, 3, 4, 5, 6};
std::mdspan<int, std::dextents<int, 2>> matrix_2x3(data.data(), 2, 3);
// matrix_2x3[i, j] gives row-major access
```

While `std::mdspan` is not yet widely available, our `matrix` class follows a similar design.

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Row-major vs column-major confusion | Wrong element accessed, silent data corruption | Be explicit: `i * cols + j` for row-major |
| Out-of-bounds on `std::mdspan` or raw arrays | No bounds checking in release builds | Use `at()` or add assertions in debug builds |
| Memory overhead of COO sparse format (3 arrays) | 3× storage vs CSR for no benefit | Use CSR for matrices with many rows per non-zero |
| Not checking dimension compatibility in multiply | Wrong result size, index out of range | Always validate `A.cols == B.rows` before multiply |
| Assuming C-style arrays decay to `std::mdspan` | Losing dimension info, wrong strides | Wrap in `std::mdspan` or `std::span` to carry extents |
| Using `vector<vector<T>>` for 2D data | Poor cache locality, fragmentation | Use flat 1D array with `i * cols + j` indexing |

## Summary

1. **Row-major order** is the standard C++ memory layout for 2D arrays: index(i,j) = i·c + j.
2. **Special matrices** (diagonal, triangular, tridiagonal) can be stored in O(n) space instead of O(n²).
3. **Sparse matrices** (CSR, COO) store only non-zero elements, enabling operations on matrices too large for dense storage.
4. **Sparse matrix-vector multiplication** runs in O(nnz) time — essential for iterative methods like PageRank.
5. The **operator()(i,j)** convention is standard for C++ matrix classes.

## Exercises

### Drill

1. For a 5×4 matrix stored in row-major order, what is the offset of element (2, 3) from the start of the array? What about for column-major order?

2. How many non-zero elements must a 1000×1000 matrix have before the sparse representation (using CSR) uses more memory than the dense representation? Assume 8 bytes per value, 8 bytes per column index, 4 bytes per row pointer, and a 64-bit machine.

3. Show the storage layout (in memory) for the special matrices:
   a) A 5×5 diagonal matrix with values [1, 2, 3, 4, 5]
   b) A 4×4 lower triangular matrix
   c) A 4×4 tridiagonal matrix

4. What is the number of non-zero elements in an n×n tridiagonal matrix? An n×n upper triangular matrix? An n×n symmetric matrix where only the lower triangle is stored?

### Application

5. Implement `tridiagonal_matrix<T>` storing only the three non-zero diagonals. Provide a `solve` method using the Thomas algorithm (O(n) time). Test it on a random tridiagonal system.

6. Write a function to multiply two CSR matrices A (m×k) and B (k×n). Analyze its time complexity in terms of non-zero counts. Compare with standard dense multiplication.

7. Implement the `toeplitz_matrix` class where each diagonal is constant. What is the minimal storage? Implement matrix-vector multiply.

8. Benchmark dense vs CSR matrix-vector multiply for matrices of varying sparsity (0.1%, 1%, 10%, 50%) at size 1000×1000. At what density does dense become faster?

9. Implement the Floyd-Warshall all-pairs shortest path algorithm on a dense matrix. Then implement it using CSR. Compare performance for sparse graphs with 1%, 5%, and 20% edge density.

10. Implement matrix transposition for CSR format — produce A^T from A in O(nnz) time.

11. Write a function to convert a dense matrix to CSR format and back. Verify correctness by comparing original and round-tripped matrices.

12. Implement the PageRank algorithm using sparse matrix-vector multiplication. Test on a graph with 1000 nodes and ~5000 edges.

13. Write a function `add_sparse` that adds two CSR matrices. What is the complexity?

### Research

14. **(SuiteSparse)** Read about the SuiteSparse Matrix Collection. Pick a real sparse matrix (e.g., from circuit simulation or web graphs) and compare CSR vs dense performance for a computation of your choice.

15. **(Cache-oblivious matrices)** Research the recursive (cache-oblivious) matrix multiplication algorithm (Z-Morton order, or recursive block decomposition). Implement it and compare with the standard triple-loop for large matrices.

16. **(SIMD matrix multiply)** Study how BLAS libraries (like Intel MKL or OpenBLAS) use SIMD instructions (AVX, AVX-512) to accelerate matrix multiplication. Implement a tiled matrix multiply that uses AVX intrinsics.

17. **(GraphBLAS)** The GraphBLAS standard defines graph algorithms in terms of linear algebra operations over semirings. Read the GraphBLAS specification and implement breadth-first search using sparse matrix-vector multiplication.

## References and Further Reading

- Gene H. Golub and Charles F. Van Loan, *Matrix Computations*, 4th Edition. JHU Press, 2013.
- Tim Davis, *Direct Methods for Sparse Linear Systems*. SIAM, 2006.
- Tim Davis, "Algorithm 832: UMFPACK V4.3 — an unsymmetric-pattern multifrontal method" — ACM TOMS, 2004.
- Yousef Saad, *Iterative Methods for Sparse Linear Systems*, 2nd Edition. SIAM, 2003.
- James W. Demmel, *Applied Numerical Linear Algebra*. SIAM, 1997.
- Jeremy Kepner and John Gilbert (eds.), *Graph Algorithms in the Language of Linear Algebra*. SIAM, 2011.
- SuiteSparse Matrix Collection — https://sparse.tamu.edu/
