#include <mdspan>
#include <vector>

std::vector<int> data = {1, 2, 3, 4, 5, 6};
std::mdspan<int, std::dextents<int, 2>> matrix_2x3(data.data(), 2, 3);
// matrix_2x3[i, j] gives row-major access
