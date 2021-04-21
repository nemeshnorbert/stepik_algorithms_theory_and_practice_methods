#include <iostream>
#include <limits>
#include <string>
#include <vector>

std::size_t get_editing_distance(const std::string& from, const std::string& to) {
  const auto n = from.size();
  const auto m = to.size();
  auto table = std::vector<std::vector<std::size_t>>(
    n + 1, std::vector<std::size_t>(m + 1, std::numeric_limits<std::size_t>::max()));
  for (auto idx = std::size_t{0}; idx <= n; ++idx) {
    table[idx][0] = idx;
  }
  for (auto jdx = std::size_t{0}; jdx <= m; ++jdx) {
    table[0][jdx] = jdx;
  }
  for (auto idx = std::size_t{1}; idx <= n; ++idx) {
    for (auto jdx = std::size_t{1}; jdx <= m; ++jdx) {
      auto distance = std::min(table[idx - 1][jdx] + 1, table[idx][jdx - 1] + 1);
      const auto replacement = (to[jdx - 1] != from[idx - 1]);
      distance = std::min(
        distance, table[idx - 1][jdx - 1] + (replacement ? std::size_t{1} : std::size_t{0}));
      table[idx][jdx] = distance;
    }
  }
  return table[n][m];
}


int main() {
  auto from = std::string{};
  auto to = std::string{};
  std::cin >> from >> to;
  std::cout << get_editing_distance(from, to);
  return 0;
}