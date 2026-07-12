std::vector<int> v = {1, 2, 3};
// Back inserter
std::fill_n(std::back_inserter(v), 3, 0);  // appends 0, 0, 0

// Stream iterator
std::copy(v.begin(), v.end(),
          std::ostream_iterator<int>(std::cout, " "));
