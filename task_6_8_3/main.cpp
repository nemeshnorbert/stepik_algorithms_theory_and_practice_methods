#include <iostream>
#include <array>

int main() {
    auto counts = std::array<int, 11>{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    auto n = std::size_t{0};
    std::cin >> n;
    for (auto i = std::size_t{0}; i < n; ++i) {
        auto value = std::size_t{0};
        std::cin >> value;
        ++counts[value];
    }
    for (auto i = std::size_t{0}; i < counts.size(); ++i) {
        for (auto cnt = 0; cnt < counts[i]; ++cnt) {
            std::cout << i << ' ';
        }
    }
    return 0;
}