#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

std::vector<std::size_t> get_longest_increasing_subsequence(
  const std::vector<int>& values) {
  if (values.empty()) {
    return std::vector<std::size_t>{};
  }
  constexpr const auto SENTINEL = std::numeric_limits<std::size_t>::max();
  const auto size = values.size();
  auto lds_prev_idx = std::vector<std::size_t>(size, SENTINEL);
  auto lds_last_idx = std::vector<std::size_t>(size + 1, SENTINEL);
  auto lds_length = std::size_t{0};
  for (auto idx = std::size_t{0}; idx < size; ++idx) {
    // binary search for the largest positive index jdx <= lds_length
    // such that values[lds_last_idx[jdx]] <= values[idx]
    auto low = std::size_t{1};
    auto high = lds_length;
    while (low <= high) {
      auto middle = low + (high - low) / 2;
      if (values[lds_last_idx[middle]] >= values[idx]) {
        low = middle + 1;
      } else {
        high = middle - 1;
      }
    }
    // After searching, low is 1 greater than the
    // length of the longest prefix of values[0...idx]
    auto new_lds_length = low;
    // The predecessor of values[idx] is the last index of
    // the subsequence of length new_lds_length - 1
    lds_prev_idx[idx] = lds_last_idx[new_lds_length - 1];
    lds_last_idx[new_lds_length] = idx;

    if (new_lds_length > lds_length) {
      lds_length = new_lds_length;
    }
  }

  // Reconstruct the longest nondecreasing subsequence
  auto route = std::vector<std::size_t>(lds_length, SENTINEL);
  auto prev_idx = lds_last_idx[lds_length];
  for (auto idx = std::size_t{0}; idx < lds_length; ++idx) {
    route[lds_length - 1 - idx] = prev_idx;
    prev_idx = lds_prev_idx[prev_idx];
  }
  return route;
}

std::vector<int> read_input(std::istream& in) {
  auto values_count = std::size_t{0};
  in >> values_count;
  auto values = std::vector<int>{};
  values.reserve(values_count);
  for (auto idx = std::size_t{0}; idx < values_count; ++idx) {
    auto value = int{0};
    in >> value;
    values.emplace_back(value);
  }
  return values;
}

template <typename T>
void write_output(std::ostream& out, std::vector<T>& route) {
  out << route.size() << '\n';
  for (const auto& idx : route) {
    out << idx << ' ';
  }
}

int main() {
  auto values = read_input(std::cin);
  auto route = get_longest_increasing_subsequence(values);
  for (auto& idx : route) {
    ++idx;
  }
  write_output(std::cout, route);
  return 0;
}