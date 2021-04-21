#include <cassert>
#include <iostream>
#include <vector>

namespace binary_heap {

// Min-heap
template <typename T, typename Comp = std::less<T>>
class binary_heap {
 public:
  using value_type = T;
  using allocator_type = std::allocator<T>;
  using value_compare = Comp;

  using size_type = typename allocator_type::size_type;
  using reference = typename allocator_type::reference;
  using const_reference = typename allocator_type::const_reference;
  using pointer = typename allocator_type::pointer;
  using const_pointer = typename allocator_type::const_pointer;

  binary_heap()
    : binary_heap{Comp{}}
  {}

  explicit binary_heap(const Comp& comp)
    : values_{}
    , comp_{comp}
  {}

  template <typename Iter>
  binary_heap(Iter first, Iter last, const Comp& comp = Comp{})
    : binary_heap{comp} {
    this->insert(first, last);
  }

  binary_heap(std::initializer_list<T> init, const Comp& comp = Comp{})
    : binary_heap{std::begin(init), std::end(init), comp}
  {}

  binary_heap<T, Comp>& operator = (std::initializer_list<T> init) {
    this->swap(binary_heap(init));
    return *this;
  }

  const_reference top() const {
    return this->values_[0];
  }

  bool empty() const {
    return this->values_.empty();
  }

  size_type size() const {
    return this->values_.size();
  }

  void clear() {
    this->values_.clear();
  }

  void push(const value_type& value) {
    this->values_.push_back(value);
    this->sift_up(this->values_.size() - 1);
  }

  template <typename ... Args>
  void emplace(Args&&... args) {
    this->values_.emplace_back(std::forward<Args>(args)...);
    this->sift_up(this->values_.size() - 1);
  }

  void pop() {
    if (this->empty()) {
      assert(false);
    }
    this->swap_nodes(0, this->values_.size() - 1);
    this->values_.pop_back();
    this->sift_down(0);
  }

  // XXX
  const std::vector<T>& get_data() const {
    return this->values_;
  }

  std::vector<T>& get_data() {
    return this->values_;
  }
  // XXX

  void swap(binary_heap<T, Comp>& other) {
    std::swap(this->values_, other.values_);
    std::swap(this->comp_, other.comp_);
  }

 private:
  template <typename Iter>
  void insert(Iter first, Iter last) {
    for (auto it = first; it != last; ++it) {
      this->push(*it);
    }
  }

  void sift_up(std::size_t node_index) {
    while (!is_root(node_index)) {
      auto parent_index = get_parent_index(node_index);
      if (this->compare_nodes(node_index, parent_index)) {
        this->swap_nodes(node_index, parent_index);
        node_index = parent_index;
      } else {
        break;
      }
    }
  }

  void sift_down(std::size_t node_index) {
    const auto nodes_count = this->values_.size();
    while (!is_leaf(node_index, nodes_count)) {
      const auto has_left = has_left_child(node_index, nodes_count);
      const auto has_right = has_right_child(node_index, nodes_count);
      if (has_left && has_right) {
        auto left_child_index = get_left_child_index(node_index);
        auto right_child_index = get_right_child_index(node_index);
        if (this->compare_nodes(left_child_index, right_child_index)) {
          if (this->compare_nodes(left_child_index, node_index)) {
            this->swap_nodes(left_child_index, node_index);
            node_index = left_child_index;
          } else {
            break;
          }
        } else {
          if (this->compare_nodes(right_child_index, node_index)) {
            this->swap_nodes(right_child_index, node_index);
            node_index = right_child_index;
          } else {
            break;
          }
        }
      } else if (has_left) {
        auto left_child_index = get_left_child_index(node_index);
        if (this->compare_nodes(left_child_index, node_index)) {
          this->swap_nodes(left_child_index, node_index);
          node_index = left_child_index;
        } else {
          break;
        }
      } else if (has_right) {
        auto right_child_index = get_right_child_index(node_index);
        if (this->compare_nodes(right_child_index, node_index)) {
          this->swap_nodes(right_child_index, node_index);
          node_index = right_child_index;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }

  bool compare_nodes(std::size_t lhs_index, std::size_t rhs_index) {
    return this->comp_(this->values_[lhs_index], this->values_[rhs_index]);
  }

  void swap_nodes(std::size_t lhs_index, std::size_t rhs_index) {
    std::swap(this->values_[lhs_index], this->values_[rhs_index]);
  }

  static std::size_t get_left_child_index(std::size_t node_index) {
    return 2 * node_index + 1;
  }

  static std::size_t get_right_child_index(std::size_t node_index) {
    return 2 * node_index + 2;
  }

  static std::size_t get_parent_index(std::size_t node_index) {
    return (node_index - 1) / 2;
  }

  static bool is_root(std::size_t node_index) {
    return node_index == 0;
  }

  static bool has_left_child(std::size_t node_index, std::size_t nodes_count) {
    return get_left_child_index(node_index) < nodes_count;
  }

  static bool has_right_child(std::size_t node_index, std::size_t nodes_count) {
    return get_right_child_index(node_index) < nodes_count;
  }

  static bool is_leaf(std::size_t node_index, std::size_t nodes_count) {
    return !has_left_child(node_index, nodes_count) && !has_right_child(node_index, nodes_count);
  }

 private:
  std::vector<T> values_;
  value_compare comp_;
};

} // namespace binary_heap



int run() {
  std::ostream& out = std::cout;
  std::istream& in = std::cin;
  auto heap = binary_heap::binary_heap<int, std::greater<int>>{};
  auto commands_count = std::size_t{0};
  in >> commands_count;
  for (auto command_id = std::size_t{0}; command_id < commands_count; ++command_id) {
    auto command_name = std::string{};
    in >> command_name;
    if (command_name == "Insert") {
      auto value = int{0};
      in >> value;
      heap.push(value);
    } else if (command_name == "ExtractMax") {
      if (heap.empty()) {
        out << "The heap is empty!\n";
      } else {
        std::cout << heap.top() << '\n';
        heap.pop();
      }
    } else {
      out << "Unknown command: " << command_name << '\n';
    }
  }
  return 0;
}

int main(){
  run();
  return 0;
}
