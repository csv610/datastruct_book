// For LCM, use __int128 or modular arithmetic
// lcm(a, b) = a / gcd(a, b) * b
// Apply mod at each step to prevent overflow
int lcm_mod(int a, int b, int mod) {
    return (long long)a / std::gcd(a, b) * b % mod;
}
