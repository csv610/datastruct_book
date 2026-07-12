// Access element (i, j) in compact storage
// Using lower triangle: index = i*(i+1)/2 + j   for i >= j
// Symmetric: A(i,j) = A(j,i), so swap if i < j
T symmetric_access(const T* data, size_t i, size_t j, size_t n) {
    if (i < j) std::swap(i, j);
    return data[i * (i + 1) / 2 + j];
}
