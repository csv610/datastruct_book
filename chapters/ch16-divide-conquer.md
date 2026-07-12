# Divide and Conquer

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The General Method

**Divide and conquer** solves a problem by:
1. **Divide:** break the problem into smaller subproblems of the same type
2. **Conquer:** solve each subproblem recursively
3. **Combine:** merge the solutions into the overall solution

```
T(n) = a·T(n/b) + f(n)

where:
a = number of subproblems
n/b = size of each subproblem
f(n) = cost of divide + combine
```

## Defective Chessboard

**Problem:** A 2ᵏ × 2ᵏ chessboard has one defective square. Cover the remaining squares with L-shaped trominoes (3 squares each).

Divide the board into four quadrants. Place one tromino at the center to "defect" the three quadrants that don't have the original defect. Recurse on each quadrant.

```cpp
// Place a tromino at (tr, tc) within a quadrant
enum quadrant { TL, TR, BL, BR };

void cover_board(std::vector<std::vector<int>>& board,
                 size_t row, size_t col,       // top-left corner of sub-board
                 size_t defect_r, size_t defect_c,  // defect position
                 size_t size,                   // sub-board dimension
                 int& tile) {                   // current tile number
    if (size == 1) return;

    size_t half = size / 2;
    int t = tile++;

    // Top-left quadrant
    if (defect_r < row + half && defect_c < col + half) {
        cover_board(board, row, col, defect_r, defect_c, half, tile);
    } else {
        board[row + half - 1][col + half - 1] = t;
        cover_board(board, row, col, row + half - 1, col + half - 1, half, tile);
    }

    // Top-right quadrant
    if (defect_r < row + half && defect_c >= col + half) {
        cover_board(board, row, col + half, defect_r, defect_c, half, tile);
    } else {
        board[row + half - 1][col + half] = t;
        cover_board(board, row, col + half, row + half - 1, col + half, half, tile);
    }

    // Bottom-left quadrant
    if (defect_r >= row + half && defect_c < col + half) {
        cover_board(board, row + half, col, defect_r, defect_c, half, tile);
    } else {
        board[row + half][col + half - 1] = t;
        cover_board(board, row + half, col, row + half, col + half - 1, half, tile);
    }

    // Bottom-right quadrant
    if (defect_r >= row + half && defect_c >= col + half) {
        cover_board(board, row + half, col + half, defect_r, defect_c, half, tile);
    } else {
        board[row + half][col + half] = t;
        cover_board(board, row + half, col + half, row + half, col + half, half, tile);
    }
}
```

**Complexity:** T(n) = 4T(n/2) + O(1) → T(n) = O(n²) = O(4ᵏ), which is optimal (there are exactly (4ᵏ - 1)/3 squares to cover).

## Binary Search

```cpp
template <std::random_access_iterator Iter, typename T>
Iter binary_search_iter(Iter first, Iter last, const T& value) {
    while (first < last) {
        auto mid = first + (last - first) / 2;
        if (*mid == value) return mid;
        if (*mid < value) first = mid + 1;
        else last = mid;
    }
    return last;  // not found
}
```

**Recurrence:** T(n) = T(n/2) + O(1) → T(n) = O(log n).

## Finding Min and Max

**Problem:** Find both the minimum and maximum of an array using fewer than 2n - 2 comparisons.

**Divide and conquer approach:** 
- Divide array in half
- Recursively find min and max of each half
- Combine: overall min = min(left_min, right_min), overall max = max(left_max, right_max)

```cpp
std::pair<int, int> min_max(std::span<const int> arr) {
    if (arr.size() == 1) return {arr[0], arr[0]};
    if (arr.size() == 2) {
        return {std::min(arr[0], arr[1]), std::max(arr[0], arr[1])};
    }

    size_t mid = arr.size() / 2;
    auto [lmin, lmax] = min_max(arr.subspan(0, mid));
    auto [rmin, rmax] = min_max(arr.subspan(mid));

    return {std::min(lmin, rmin), std::max(lmax, rmax)};
}
```

**Comparisons:** T(n) = 2T(n/2) + 2 → T(n) = 3n/2 - 2. This is optimal for finding both min and max simultaneously.

## Sorting Algorithms

### Merge Sort

Divide array in half, sort each half, merge.

```cpp
void merge_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;

    size_t mid = arr.size() / 2;
    std::vector<int> left(arr.begin(), arr.begin() + mid);
    std::vector<int> right(arr.begin() + mid, arr.end());

    merge_sort(left);
    merge_sort(right);

    // Merge
    size_t i = 0, j = 0, k = 0;
    while (i < left.size() && j < right.size()) {
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    }
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}
```

**Recurrence:** T(n) = 2T(n/2) + O(n) → T(n) = O(n log n).  
**Space:** O(n) extra — merge sort is not in-place.

### Natural Merge Sort

A variant that identifies existing sorted runs in the data. If the input is already sorted, natural merge sort runs in O(n).

### Quick Sort

Choose a **pivot**, partition the array into elements ≤ pivot and ≥ pivot, recurse on each partition.

```cpp
size_t partition(std::span<int> arr) {
    // Lomuto partition scheme
    int pivot = arr.back();
    size_t i = 0;
    for (size_t j = 0; j < arr.size() - 1; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr.back());
    return i;
}

void quick_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;
    size_t p = partition(arr);
    quick_sort(arr.subspan(0, p));
    quick_sort(arr.subspan(p + 1));
}
```

**Average:** T(n) = O(n log n). **Worst case (already sorted):** O(n²).  
Mitigated by random pivot selection:

```cpp
size_t random_partition(std::span<int> arr) {
    size_t r = std::rand() % arr.size();
    std::swap(arr[r], arr.back());
    return partition(arr);
}
```

**Space:** O(log n) for the recursion stack (worst case O(n)).

### Comparison

| Algorithm | Worst Case | Average | Best Case | Space | Stable |
|-----------|-----------|---------|-----------|-------|--------|
| Merge sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick sort | O(n²) | O(n log n) | O(n log n) | O(log n) | No |
| Heap sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Introsort | O(n log n) | O(n log n) | O(n log n) | O(log n) | No |

**Introsort** (used by `std::sort`) starts with quicksort, switches to heapsort if recursion depth exceeds log n, and uses insertion sort for small arrays.

## Selection (Finding the k-th Smallest)

**Quickselect:** Partition the array around a pivot, then recurse only on the partition containing the k-th element.

```cpp
int quick_select(std::span<int> arr, size_t k) {
    if (arr.size() == 1) return arr[0];

    size_t p = random_partition(arr);

    if (k == p) return arr[p];
    if (k < p) return quick_select(arr.subspan(0, p), k);
    return quick_select(arr.subspan(p + 1), k - p - 1);
}
```

**Average:** O(n). **Worst:** O(n²).  
The **Median of Medians** algorithm achieves O(n) worst-case selection.

## Closest Pair of Points

**Problem:** Given n points in the plane, find the pair with the smallest Euclidean distance.

**Divide and conquer approach:**
1. Sort points by x-coordinate
2. Divide at median x
3. Recursively find d = min(closest in left, closest in right)
4. Consider the strip of width 2d around the median — only O(n) points to check

```cpp
struct point { double x, y; };

double closest_pair(std::span<point> points) {
    if (points.size() <= 3) return brute_force(points);

    size_t mid = points.size() / 2;
    double mid_x = points[mid].x;

    double dl = closest_pair(points.subspan(0, mid));
    double dr = closest_pair(points.subspan(mid));
    double d = std::min(dl, dr);

    // Collect points within the strip
    std::vector<point> strip;
    for (const auto& p : points) {
        if (std::abs(p.x - mid_x) < d) strip.push_back(p);
    }

    // Sort strip by y
    std::sort(strip.begin(), strip.end(),
              [](const point& a, const point& b) { return a.y < b.y; });

    // Check each point against next few (at most 7)
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; ++j) {
            double dist = euclidean(strip[i], strip[j]);
            d = std::min(d, dist);
        }
    }
    return d;
}
```

**Complexity:** T(n) = 2T(n/2) + O(n log n) [sorting by y each level] → O(n log² n).  
**Optimized:** Pre-sort by y once → T(n) = 2T(n/2) + O(n) → O(n log n).

## Strassen's Matrix Multiplication

Standard matrix multiplication: 8 multiplications of n/2 × n/2 submatrices → T(n) = 8T(n/2) + O(n²) → O(n³).

Strassen's algorithm reduces multiplication count to 7:

```
M₁ = (A₁₁ + A₂₂) · (B₁₁ + B₂₂)
M₂ = (A₂₁ + A₂₂) · B₁₁
M₃ = A₁₁ · (B₁₂ - B₂₂)
M₄ = A₂₂ · (B₂₁ - B₁₁)
M₅ = (A₁₁ + A₁₂) · B₂₂
M₆ = (A₂₁ - A₁₁) · (B₁₁ + B₁₂)
M₇ = (A₁₂ - A₂₂) · (B₂₁ + B₂₂)

C₁₁ = M₁ + M₄ - M₅ + M₇
C₁₂ = M₃ + M₅
C₂₁ = M₂ + M₄
C₂₂ = M₁ - M₂ + M₃ + M₆
```

**Complexity:** T(n) = 7T(n/2) + O(n²) → T(n) = O(n^(log₂7)) ≈ O(n^(2.807)).

## Common Bugs and Pitfalls

- **Incorrect base case** — forgetting the base case (n ≤ 1) causes infinite recursion and stack overflow. Always verify that every recursive call moves toward the base case.
- **Expensive combine step** — if the combine step is O(n²), a divide-and-conquer algorithm may be slower than a naive solution. This is why the closest-pair algorithm must carefully limit the strip check to O(n).
- **Quick sort with bad pivot selection** — always using the first/last element as pivot causes O(n²) on sorted/almost-sorted data. Use median-of-three or random pivots in production.
- **Merge sort's extra memory** — merge sort requires O(n) additional space. In-place merge sort exists but is complex and rarely used. For systems with tight memory (embedded), consider heap sort or introsort instead.
- **Strassen's numerical stability** — Strassen's algorithm uses subtractions that can cause catastrophic cancellation for ill-conditioned matrices. Production matrix libraries (BLAS, Eigen) use Strassen only for large matrices where the error is bounded.

## Summary

1. **Divide and conquer** breaks a problem into independent subproblems, solves them recursively, and combines solutions.
2. **Binary search** is the simplest and most widely used divide-and-conquer algorithm.
3. **Merge sort** and **quick sort** are the canonical divide-and-conquer sorts.
4. **Closest pair** and **Strassen's multiplication** demonstrate non-trivial divide and combine steps.
5. Industrial applications include **merge sort in PostgreSQL** (external sorting), **quick sort in C++ std::sort** (introsort hybrid), **Strassen in matrix libraries**, **FFT in JPEG/MP3**, **binary search in database indexes**, and **quickselect in Netflix's recommendation rankings**.
6. The **Master Theorem** (Chapter 2) provides the complexity framework.

## Exercises

### Drill

1. Solve T(n) = 2T(n/2) + n, T(n) = T(n/2) + 1, and T(n) = 7T(n/2) + n². Which Master Theorem case applies to each?

2. Trace merge sort on [3, 1, 4, 1, 5, 9, 2, 6]. Show the array at each recursion level.

3. Trace quick sort on [3, 1, 4, 1, 5, 9, 2, 6] using the last element as pivot. Show the array after each partition step.

4. In PostgreSQL's external merge sort, data spills to disk when it doesn't fit in memory. If the sort requires 10GB of memory but only 1GB is available, how many merge passes are needed? What is the total I/O cost?

5. What is the worst-case input for quick sort? For merge sort? For binary search? In each case, give a real scenario where that worst case arises in production.

### Application

6. **PostgreSQL external merge sort.** Implement an external merge sort that sorts a file larger than RAM. Split the input into chunks that fit in memory, sort each chunk (internal sort), write to temporary files, then merge the sorted runs. Test on a synthetic 1GB file with 100MB memory limit. Measure I/O volume and wall time.

7. **C++ std::sort introsort.** `std::sort` uses introsort: quick sort with a depth limit, falling back to heap sort when recursion exceeds 2·log₂(n). Implement introsort and benchmark it against plain quick sort on sorted, reversed, and random arrays of 10⁷ elements. Show that introsort avoids the O(n²) worst case.

8. **Netflix recommendation ranking (quickselect).** Netflix needs to find the top-100 movies from 10,000 candidates for each user. Implement quickselect to find the 100th highest score, then partition around it. Benchmark vs sorting all 10,000. At what k does sorting become cheaper than quickselect?

9. **FFT for audio processing.** Implement the Cooley-Tukey FFT for polynomial multiplication. Measure the time to multiply two degree-1,048,575 polynomials. Compare with naive O(n²) multiplication. Show the crossover point where FFT becomes faster.

10. **Karatsuba integer multiplication for cryptography.** RSA encryption uses large integers (2048+ bits). Implement Karatsuba multiplication and benchmark against GMP's built-in multiplication for 256-bit, 1024-bit, and 4096-bit integers. At what bit width does Karatsuba beat the O(n²) algorithm?

11. **Binary search in database index lookup.** MongoDB uses B-tree indexes with binary search within each node. Implement binary search on an array of 10⁷ integers and measure cache misses using `perf stat`. Compare with `std::lower_bound`. Why does branchless binary search (using `cmov` instructions) outperform the standard version for large arrays?

12. **Closest pair in GPS trajectory compression.** The Douglas-Peucker algorithm for GPS trajectory simplification uses the closest-pair idea: find the point farthest from the line between endpoints. Implement the divide-and-conquer closest-pair algorithm and apply it to a GPS trace of 100,000 points (from OpenStreetMap data). Show the compressed trajectory at various tolerance levels.

### Research

13. **(Strassen's algorithm)** Implement Strassen's algorithm with a cutoff (switch to standard multiplication for n < 64). Measure the crossover point on various matrix sizes and hardware. Does the crossover change with CPU cache size? Compare with Intel MKL's performance.

14. **(Parallel merge sort)** Implement a parallel merge sort using C++17 parallel algorithms (`std::execution::par`) and `std::thread`. Measure speedup on 4, 8, and 16 cores. What is the parallel efficiency? Where does Amdahl's law limit the speedup?

15. **(Van Emde Boas layout)** Read about the cache-oblivious van Emde Boas array layout for binary search. Implement binary search on a vEB-laid-out array and compare cache misses with standard binary search. How much does cache-oblivious layout improve performance for very large arrays?

16. **(Sorting lower bound)** Prove the Ω(n log n) lower bound for comparison sorting using decision trees. Show that merge sort achieves this bound. Then implement a non-comparison sort (counting sort or radix sort) and show that it beats the lower bound by exploiting the input structure.

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapters 4 (divide and conquer), 30 (FFT).
- Volker Strassen, "Gaussian Elimination is Not Optimal" — Numerische Mathematik, 1969.
- Jon Bentley, *Programming Pearls*, 2nd Edition. Addison-Wesley, 2000.
- Michael I. Shamos and Dan Hoey, "Closest-Point Problems" — FOCS, 1975.
- Anatolii A. Karatsuba and Yuri Ofman, "Multiplication of Many-Digital Numbers by Automatic Computers" — Doklady Akademii Nauk SSSR, 1962.
- James W. Cooley and John W. Tukey, "An Algorithm for the Machine Calculation of Complex Fourier Series" — Mathematics of Computation, 1965.
