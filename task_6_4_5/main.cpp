#include <algorithm>
#include <iterator>
#include <cassert>
#include <iostream>
#include <vector>
#include <set>

class merge_sorter {
 public:
  merge_sorter()
    : swap_count_{0} {
    set();
  }

  void set() {
    this->swap_count_ = std::size_t{0};
  }

  template <typename Container>
  void sort(Container& cont) {
    return this->sort(std::begin(cont), std::end(cont));
  }

  template <typename Iter>
  void sort(Iter first, Iter last) {
    assert(first <= last);
    using value_type = std::decay_t<decltype(*first)>;
    const auto buffer_size = static_cast<std::size_t>(last - first);
    auto buffer = std::vector<value_type>(buffer_size);
    sort(first, last, std::begin(buffer), std::end(buffer));
  }

  std::size_t swap_count() const {
    return this->swap_count_;
  }

  void reset() {
    this->swap_count_ = std::size_t{0};
  }

 private:
  template <typename Iter>
  void sort(Iter first, Iter last, Iter out_first, Iter out_last) {
    const auto sort_size = last - first;
    const auto out_size = out_last - out_first;
    assert(sort_size == out_size);
    constexpr auto MIN_MERGE_SORT_SIZE = 8;
    if (sort_size >= MIN_MERGE_SORT_SIZE) {
      auto middle = first + (last - first) / 2;
      auto out1_first = out_first;
      auto out1_last = out_first + (middle - first);
      this->sort(first, middle, out1_first, out1_last);
      auto out2_first = out1_last;
      auto out2_last = out_last;
      this->sort(middle, last, out2_first, out2_last);
      this->merge(first, middle, middle, last, out_first, out_last);
      std::copy(out_first, out_last, first);
    } else {
      this->insertion_sort(first, last);
    }
  }

  template <typename Iter>
  void insertion_sort(Iter first, Iter last) {
    for (auto it = std::next(first); it != last; ++it) {
      auto cur = it;
      while(cur != first) {
        auto prev = std::prev(cur);
        if (*cur < *prev) {
           std::swap(*cur, *prev);
           ++this->swap_count_;
           --cur;
        } else {
          break;
        }
      }
    }
  }

  template <typename Iter>
  void merge(Iter first1, Iter last1, Iter first2, Iter last2, Iter out_first, Iter out_last) {
    auto out = out_first;
    auto cur1 = first1;
    auto cur2 = first2;
    while (cur1 != last1 && cur2 != last2) {
      if (*cur1 > *cur2) {
        *out++ = *cur2++;
        this->swap_count_ += last1 - cur1;
      } else {
        *out++ = *cur1++;
      }
    }
    while (cur1 != last1) {
        *out++ = *cur1++;
    }
    while (cur2 != last2) {
        *out++ = *cur2++;
    }
  }

 private:
  std::size_t swap_count_;
};

template <typename T>
std::vector<T> read_vector(std::istream& in) {
  auto values_count = std::size_t{0};
  in >> values_count;
  auto values = std::vector<T>{};
  values.reserve(values_count);
  for (auto idx = std::size_t{0}; idx < values_count; ++idx) {
    auto value = T{};
    in >> value;
    values.push_back(value);
  }
  return values;
}

template <typename Iter>
std::size_t count_misplacements_via_sort(Iter first, Iter last) {
  using value_type = std::decay_t<decltype(*first)>;
  auto values = std::vector<value_type>{first, last};
  auto sorter = merge_sorter{};
  sorter.sort(values);
  return sorter.swap_count();
}

template <typename Container>
std::size_t count_misplacements_via_sort(const Container& cont) {
  return count_misplacements_via_sort(std::begin(cont), std::end(cont));
}

template <typename Iter>
std::size_t count_misplacements_via_brute_force(Iter first, Iter last) {
  auto count = std::size_t{0};
  for (auto i = first; i != last; ++i) {
    for (auto j = std::next(i); j != last; ++j) {
      if (*i > *j) {
        ++count;
      }
    }
  }
  return count;
}

template <typename Container>
std::size_t count_misplacements_via_brute_force(const Container& cont) {
  return count_misplacements_via_brute_force(std::begin(cont), std::end(cont));
}

std::vector<int> generate_random_vector(std::size_t count) {
  auto unique_values = std::set<int>{};
  while (unique_values.size() < count) {
    unique_values.insert(rand() % count);
  }
  auto values = std::vector<int>(std::begin(unique_values), std::end(unique_values));
  std::random_shuffle(std::begin(values), std::end(values));
  return values;
}

void run_stress_test() {
  for (auto attempt = std::size_t{0}; attempt < 100; ++attempt) {
    auto values = generate_random_vector(16);
    auto fast_count = count_misplacements_via_sort(values);
    auto slow_count = count_misplacements_via_brute_force(values);
    if (fast_count == slow_count) {
      std::cout << "attempt = " << attempt << '\n';
      std::copy(std::begin(values), std::end(values), std::ostream_iterator<int>(std::cout, " "));
      std::cout << "\nfast_count = " << fast_count << '\n';
      std::cout << "slow_count = " << slow_count << '\n';
    }
  }
}

void run_application() {
  auto values = read_vector<int>(std::cin);
  std::cout << count_misplacements_via_sort(values);
}

int main() {
  run_application();
  return 0;
}
