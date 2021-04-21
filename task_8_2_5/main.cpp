#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

std::vector<std::size_t> read_input(std::istream& in) {
  auto values_count = std::size_t{0};
  in >> values_count;
  auto values = std::vector<std::size_t>{};
  values.reserve(values_count);
  for (auto idx = std::size_t{0}; idx < values_count; ++idx) {
    auto value = std::size_t{0};
    in >> value;
    assert(value >= 1);
    values.emplace_back(value);
  }
  return values;
}

std::size_t get_longest_divisor_subsequence_length(const std::vector<std::size_t>& values) {
  if (values.empty()) {
    return 0;
  }
  auto lds_lengths = std::vector<std::size_t>(values.size(), 1);
  const auto size = lds_lengths.size();
  for (auto idx = std::size_t{0}; idx < size; ++idx) {
    for (auto jdx = std::size_t{0}; jdx < idx; ++jdx) {
      if (values[idx] % values[jdx] == 0) {
        lds_lengths[idx] = std::max(lds_lengths[idx], 1 + lds_lengths[jdx]);
      }
    }
  }
  return *std::max_element(std::begin(lds_lengths), std::end(lds_lengths));
}

int main() {
  auto values = read_input(std::cin);
  std::cout << get_longest_divisor_subsequence_length(values);
  return 0;
}