void hanoi(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);           // step 1: move n-1 to aux
    std::cout << "Move disk " << n
              << " from " << from << " to " << to << "\n";
    hanoi(n - 1, aux, to, from);           // step 3: move n-1 to target
}
