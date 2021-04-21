#include <iostream>
#include <cmath>

int main() {
    auto n = 0;
    std::cin >> n;
    auto k = static_cast<int>(std::sqrt(2. * n + 0.25) - 0.5);
    std::cout << k << '\n';
    for (auto i = 1; i < k; ++i) {
        std::cout << i << ' ';
    }
    std::cout << n - k * (k - 1) / 2;
    return 0;
}