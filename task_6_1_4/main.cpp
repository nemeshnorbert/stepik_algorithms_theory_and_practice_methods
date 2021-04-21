#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

template <typename Iter, typename T>
Iter bin_search(Iter first, Iter last, const T& value) {
  auto begin = first;
  auto end = last;
  while (begin < end) {
      auto middle = begin + (end - begin) / 2;
      const auto& pivot = *middle;
      if (pivot == value) {
          return middle;
      } else if (pivot < value) {
          begin = middle + 1;
      } else {
          end = middle;
      }
  }
  return last;
}

int get_pos(const std::vector<int>& numbers, int number) {
  auto begin = std::begin(numbers);
  auto end = std::end(numbers);
  auto here = bin_search(begin, end, number);
  if (here == end) {
      return -1;
  }
  return here - begin + 1;
}

int main(void) {
  size_t number_count;
  std::cin >> number_count;
  std::vector<int> numbers(number_count);
  for (size_t i = 0; i < number_count; i++) {
    std::cin >> numbers[i];
  }

  size_t query_count;
  std::cin >> query_count;
  while (query_count-- > 0) {
    int number;
    std::cin >> number;
    std::cout << get_pos(numbers, number) << " ";
  }
  std::cout << std::endl;
}
