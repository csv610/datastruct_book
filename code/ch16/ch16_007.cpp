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
