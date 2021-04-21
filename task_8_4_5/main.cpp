#include <iostream>
#include <vector>

std::size_t get_max_knapsack_weight(
  const std::vector<std::size_t>& weights, const std::size_t knapsack_capacity) {
  const auto bullion_count = weights.size();
  auto table = std::vector<std::vector<std::size_t>>(
    knapsack_capacity + 1, std::vector<std::size_t>(bullion_count + 1, 0));
  for (auto count = std::size_t{1}; count <= bullion_count; ++count) {
    for (auto capacity = std::size_t{1}; capacity <= knapsack_capacity; ++capacity) {
      table[capacity][count] = table[capacity][count - 1];
      const auto bullion_id = count - 1;
      const auto weight = weights[bullion_id];
      const auto cost = weight;
      if (weight <= capacity) {
        table[capacity][count] = std::max(
          table[capacity][count],
          table[capacity - weight][count - 1] + cost);
      }
    }
  }
  return table[knapsack_capacity][bullion_count];
}


int main() {
  auto knapsack_capacity = std::size_t{0};
  std::cin >> knapsack_capacity;
  auto bullion_count = std::size_t{0};
  std::cin >> bullion_count;
  auto bullion_weights = std::vector<std::size_t>{};
  bullion_weights.reserve(bullion_count);
  for (auto bullion_id = std::size_t{0}; bullion_id < bullion_count; ++bullion_id) {
    auto weight = std::size_t{0};
    std::cin >> weight;
    bullion_weights.emplace_back(weight);
  }
  std::cout << get_max_knapsack_weight(bullion_weights, knapsack_capacity);
  return 0;
}