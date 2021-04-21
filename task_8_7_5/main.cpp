#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


class actions_counter {
 public:
  actions_counter()
    : actions_count{}
    , action{}
  {}

  void init(const std::size_t number) {
    actions_count = std::vector<uint64_t>(number + 1, 0);
    action = std::vector<uint8_t>(number + 1, 0);
    for (auto idx = std::size_t{2}; idx <= number; ++idx) {
      actions_count[idx] = actions_count[idx - 1] + 1;
      action[idx] = 1;
      if (idx % 2 == 0) {
        if (actions_count[idx / 2] + 1 < actions_count[idx]) {
          actions_count[idx] = actions_count[idx / 2] + 1;
          action[idx] = 2;
        }
      }
      if (idx % 3 == 0) {
        if (actions_count[idx / 3] + 1 < actions_count[idx]) {
          actions_count[idx] = actions_count[idx / 3] + 1;
          action[idx] = 3;
        }
      }
    }
  }

  std::vector<std::size_t> find_series(const std::size_t number) {
    assert(number < actions_count.size());
    auto idx = number;
    auto series = std::vector<std::size_t>{};
    while (idx != 0) {
      series.push_back(idx);
      if (action[idx] == 3) {
        idx /= 3;
      } else if (action[idx] == 2) {
        idx /= 2;
      } else if (action[idx] == 1) {
        idx -= 1;
      } else if (action[idx] == 0) {
        break;
      } else {
        assert(false);
      }
    }
    std::reverse(std::begin(series), std::end(series));
    return series;
  }

  void reset() {
    actions_count.clear();
    action.clear();
  }

 private:
  // actions_count[i] - minimal number of actions required to get i from 1
  std::vector<uint64_t> actions_count;
  // action[i] - optimal action to get i in one step
  // 0 -> no op
  // 1 -> x + 1
  // 2 -> 2x
  // 3 -> 3x
  std::vector<uint8_t> action;
};

int main() {
  auto number = std::size_t{0};
  std::cin >> number;
  auto counter = actions_counter{};
  counter.init(number);
  auto series = counter.find_series(number);
  assert(!series.empty());
  std::cout << series.size() - 1 << '\n';
  std::copy(std::begin(series), std::end(series), std::ostream_iterator<int>(std::cout, " "));
  return 0;
}