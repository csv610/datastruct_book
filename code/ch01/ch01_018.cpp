void process(std::span<int> data) {
    for (int& x : data) x *= 2;
}

std::vector<int> v = {1, 2, 3};
process(v);              // span from vector
int arr[] = {4, 5, 6};
process(arr);            // span from C array
