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
