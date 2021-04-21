#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

struct Item final {
  int weight;
  int value;
};

double get_max_knapsack_value(int capacity, std::vector<Item> items) {
    std::sort(
        std::begin(items), std::end(items),
        [](const Item& lhs, const Item& rhs) -> bool {
            return lhs.value * rhs.weight > rhs.value * lhs.weight;
        }
    );
    auto value = 0.;
    for (const auto& item : items) {
        if (capacity <= 0) {
            break;
        }
        if (item.weight <= capacity) {
            value += item.value;
            capacity -= item.weight;
        } else {
            value += static_cast<double>(item.value * capacity) /
                static_cast<double>(item.weight);
            capacity = 0;
        }
    }
    return value;
}

int main() {
    auto number_of_items = 0;
    auto knapsack_capacity = 0;
    std::cin >> number_of_items >> knapsack_capacity;
    auto items = std::vector<Item>(number_of_items);
    for (auto i = 0; i < number_of_items; i++) {
        std::cin >> items[i].value >> items[i].weight;
    }
    double max_knapsack_value = get_max_knapsack_value(
        knapsack_capacity, std::move(items));
    std::cout.precision(10);
    std::cout << max_knapsack_value << std::endl;
    return 0;
}