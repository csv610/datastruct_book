// O(n) input space, O(1) auxiliary space
int sum(const std::vector<int>& a) {
    int total = 0;
    for (int x : a) total += x;
    return total;
}

// O(n) input space, O(n) auxiliary space
std::vector<int> reverse_copy(const std::vector<int>& a) {
    std::vector<int> result(a.rbegin(), a.rend());
    return result;
}

// O(n) input space, O(n) auxiliary space (recursion stack)
void print_reverse(const std::vector<int>& a, int i) {
    if (i < 0) return;
    std::cout << a[i] << " ";
    print_reverse(a, i - 1);
}
