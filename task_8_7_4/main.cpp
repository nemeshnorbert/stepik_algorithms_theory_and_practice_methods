#include <iostream>
#include <vector>

class score_maximizer {
 public:
  score_maximizer()
    : scores_{}
    , initialized_{}
  {}

  std::int64_t compute_max_score(const std::vector<std::int32_t>& weights) {
    const auto steps_count = weights.size();
    scores_.resize(steps_count + 1);
    scores_.assign(steps_count + 1, -1);
    initialized_.resize(steps_count + 1);
    initialized_.assign(steps_count + 1, 0);
    return compute_max_score(steps_count, weights);
  }

 private:
  int64_t compute_max_score(
    const std::size_t step_id, const std::vector<std::int32_t>& weights) {
    if (initialized_[step_id] == 0) {
      if (step_id <= 1) {
        scores_[step_id] = step_id == 1 ? weights[step_id - 1] : 0;
      } else {
        scores_[step_id] = weights[step_id - 1] + std::max(
          compute_max_score(step_id - 1, weights), compute_max_score(step_id - 2, weights));
      }
      initialized_[step_id] = 1;
    }
    return scores_[step_id];
  }

 private:
  std::vector<std::int64_t> scores_;
  std::vector<char> initialized_;
};

int main() {
  auto steps_count = std::size_t{0};
  std::cin >> steps_count;
  auto weights = std::vector<std::int32_t>(steps_count, 0);
  for (auto idx = std::size_t{0}; idx < steps_count; ++idx) {
    auto score = std::int32_t{0};
    std::cin >> score;
    weights[idx] = score;
  }
  auto calc = score_maximizer{};
  std::cout << calc.compute_max_score(weights);
  return 0;
}