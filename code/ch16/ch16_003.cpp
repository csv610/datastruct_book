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
