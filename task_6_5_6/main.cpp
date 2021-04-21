#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

template <typename PointT>
struct segment {
  PointT left;
  PointT right;
};

template <typename PointT>
class point_counter {
 public:
  using point_collection = std::vector<PointT>;

  point_counter()
    : lefts_{}
    , rights_{}
  {}

  template <typename Iter>
  void init(Iter first, Iter last) {
    auto segments_count = last - first;
    lefts_.reserve(segments_count);
    rights_.reserve(segments_count);
    for (auto it = first; it != last; ++it) {
      lefts_.emplace_back(it->left);
      rights_.emplace_back(it->right);
    }
    std::sort(std::begin(lefts_), std::end(lefts_));
    std::sort(std::begin(rights_), std::end(rights_));
  }

  std::size_t get_count(const PointT& point) {
    auto count_le = get_count_less(rights_, point);
    auto count_leq = get_count_less_or_equal(lefts_, point);
    assert(count_leq >= count_le);
    return count_leq - count_le;
  }

  void reset() {
    lefts_.clear();
    rights_.clear();
  }

 private:
  std::size_t get_count_less(const point_collection& points, const PointT& point) {
    auto begin = std::begin(points);
    auto end = std::end(points);
    auto lbound = std::lower_bound(begin, end, point);
    return lbound - begin;
  }

  std::size_t get_count_less_or_equal(const point_collection& points, const PointT& point) {
    auto begin = std::begin(points);
    auto end = std::end(points);
    return std::upper_bound(begin, end, point) - begin;
  }

  point_collection lefts_;
  point_collection rights_;
};

int main() {
  auto segments_count = std::size_t{0};
  auto points_count = std::size_t{0};
  std::cin >> segments_count >> points_count;
  auto segments = std::vector<segment<int>>{};
  segments.reserve(segments_count);
  for (auto segment_id = std::size_t{0}; segment_id < segments_count; ++segment_id) {
    auto left = int{0};
    auto right = int{0};
    std::cin >> left >> right;
    segments.push_back(segment<int>{left, right});
  }
  auto points = std::vector<int>{};
  points.reserve(points_count);
  for (auto point_id = std::size_t{0}; point_id < points_count; ++point_id) {
    auto point = int{0};
    std::cin >> point;
    points.push_back(point);
  }
  auto counter = point_counter<int>{};
  counter.init(std::begin(segments), std::end(segments));
  for (const auto& point : points) {
    std::cout << counter.get_count(point) << ' ';
  }
  return 0;
}