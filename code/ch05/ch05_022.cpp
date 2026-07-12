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
