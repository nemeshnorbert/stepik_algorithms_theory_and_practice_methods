
#include <iostream>
#include <cassert>
#include <memory>
#include <map>
#include <utility>
#include <string>
#include <unordered_map>

namespace {

class symbol_stats : public std::map<char, std::size_t> {};

using huffman_table = std::unordered_map<char, std::string>;

class huffman_encode_tree_node {
 public:
  explicit huffman_encode_tree_node(std::size_t id, std::size_t frequency)
    : id_{id}
    , frequency_{frequency}
    , symbol_{}
    , left_{nullptr}
    , right_{nullptr} {
  }

  huffman_encode_tree_node(std::size_t id, std::size_t frequency, char symbol)
    : id_{id}
    , frequency_{frequency}
    , symbol_{symbol}
    , left_{nullptr}
    , right_{nullptr} {
  }

  huffman_encode_tree_node(
    std::size_t id,
    std::size_t frequency,
    std::unique_ptr<huffman_encode_tree_node> left,
    std::unique_ptr<huffman_encode_tree_node> right)
    : id_{id}
    , frequency_{frequency}
    , symbol_{}
    , left_{std::move(left)}
    , right_{std::move(right)} {
  }

  std::size_t get_id() const {
    return this->id_;
  }

  char get_symbol() const {
    return this->symbol_;
  }

  std::size_t get_frequency() const {
    return this->frequency_;
  }

  huffman_encode_tree_node* get_left() const {
    return this->left_.get();
  }

  huffman_encode_tree_node* get_right() const {
    return this->right_.get();
  }

  bool is_leaf() const {
    return this->left_ == nullptr && this->right_ == nullptr;
  }

 private:
  std::size_t id_;
  std::size_t frequency_;
  char symbol_;
  bool is_leaf_;
  std::unique_ptr<huffman_encode_tree_node> left_;
  std::unique_ptr<huffman_encode_tree_node> right_;
};

class huffman_encode_tree_node_comparator {
 public:
  bool operator () (
    const huffman_encode_tree_node* lhs,
    const huffman_encode_tree_node* rhs) const {
    assert(lhs != nullptr);
    assert(rhs != nullptr);

    if (lhs->get_frequency() == rhs->get_frequency()) {
      if (lhs->is_leaf() && rhs->is_leaf()) {
        return lhs->get_symbol() < rhs->get_symbol();
      } else if (lhs->is_leaf() && !rhs->is_leaf()) {
        return true;
      } else if (!lhs->is_leaf() && rhs->is_leaf()) {
        return false;
      } else {
        return lhs->get_id() < rhs->get_id();
      }
    }
    return lhs->get_frequency() < rhs->get_frequency();
  }
};

class huffman_encode_tree {
 public:
  explicit huffman_encode_tree(const symbol_stats& stats)
    : root_{nullptr} {
    this->build_tree(stats);
  }

  const huffman_encode_tree_node* get_root() const {
    return this->root_.get();
  }

 private:
  void build_tree(const symbol_stats& stats) {
    if (stats.size() == 1) {
      const auto& item = *std::begin(stats);
      const auto& symbol = item.first;
      const auto& frequency = item.second;
      auto right_node = std::make_unique<huffman_encode_tree_node>(
        0, frequency, symbol);
      this->root_ = std::make_unique<huffman_encode_tree_node>(
        1, frequency, nullptr, std::move(right_node));
      return;
    }
    auto forest = std::map<
      huffman_encode_tree_node*,
      std::unique_ptr<huffman_encode_tree_node>,
      huffman_encode_tree_node_comparator>{};
    auto id = std::size_t{0};
    for (const auto& item : stats) {
      const auto& symbol = item.first;
      const auto& frequency = item.second;
      auto node = std::make_unique<huffman_encode_tree_node>(id, frequency, symbol);
      forest[node.get()] = std::move(node);
      ++id;
    }
    const auto steps_count = forest.size();
    for (auto step = std::size_t{0}; step + 1 < steps_count; ++step) {
      auto min1_node_it = std::begin(forest);
      auto min2_node_it = std::next(min1_node_it);
      auto min1_node = std::move(min1_node_it->second);
      auto min2_node = std::move(min2_node_it->second);
      const auto min1_symbol = min1_node_it->first;
      const auto min2_symbol = min2_node_it->first;
      forest.erase(min1_node_it);
      forest.erase(min2_node_it);
      const auto new_frequency = min1_node->get_frequency() + min2_node->get_frequency();
      auto node = std::unique_ptr<huffman_encode_tree_node>{};
      if (min1_symbol < min2_symbol) {
        node = std::make_unique<huffman_encode_tree_node>(
          id, new_frequency, std::move(min1_node), std::move(min2_node));
      } else if (min2_symbol < min1_symbol) {
        node = std::make_unique<huffman_encode_tree_node>(
          id, new_frequency, std::move(min2_node), std::move(min1_node));
      } else {
        assert(min1_symbol != min2_symbol);
      }
      forest[node.get()] = std::move(node);
      ++id;
    }
    this->root_ = std::move(std::begin(forest)->second);
  }

 private:
  std::unique_ptr<huffman_encode_tree_node> root_;
};

template <typename Iter>
symbol_stats build_symbol_stats(Iter first, Iter last) {
  auto stats = symbol_stats{};
  for (auto it = first; it != last; ++it) {
    ++stats[*it];
  }
  return stats;
}

huffman_encode_tree build_encode_tree(const symbol_stats& stats) {
  return huffman_encode_tree{stats};
}

void build_table_from_node(
  huffman_table* table,
  const huffman_encode_tree_node* root,
  std::string* path_code) {
  if (root == nullptr) {
    return;
  }
  if (root->is_leaf()) {
    (*table)[root->get_symbol()] = *path_code;
  }
  path_code->push_back('0');
  build_table_from_node(table, root->get_left(), path_code);
  path_code->pop_back();
  path_code->push_back('1');
  build_table_from_node(table, root->get_right(), path_code);
  path_code->pop_back();
}

huffman_table build_table(const huffman_encode_tree& tree) {
  // const auto* root = tree.get_root();
  auto table = huffman_table{};
  // if (root->get_left() == nullptr && root->get_right() == nullptr) {
  //   table[root->get_symbol()] = std::string{"1"};
  // } else {
    auto path_code = std::string{};
    build_table_from_node(&table, tree.get_root(), &path_code);
  // }
  return table;
}

class huffman_decode_tree_node {
 public:
  huffman_decode_tree_node(char symbol)
    : symbol_{symbol}
    , left_{nullptr}
    , right_{nullptr} {
  }

  huffman_decode_tree_node()
    : symbol_{}
    , left_{nullptr}
    , right_{nullptr} {
  }

  char get_symbol() const {
    return this->symbol_;
  }

  void set_symbol(char symbol) {
    this->symbol_ = symbol;
  }

  huffman_decode_tree_node* get_left() const {
    return this->left_.get();
  }

  huffman_decode_tree_node* get_right() const {
    return this->right_.get();
  }

  void set_left(std::unique_ptr<huffman_decode_tree_node> node) {
    this->left_ = std::move(node);
  }

  void set_right(std::unique_ptr<huffman_decode_tree_node> node) {
    this->right_ = std::move(node);
  }

  bool is_leaf() const {
    return this->left_ == nullptr && this->right_ == nullptr;
  }

 private:
  char symbol_;
  std::unique_ptr<huffman_decode_tree_node> left_;
  std::unique_ptr<huffman_decode_tree_node> right_;
};

class huffman_decode_tree {
 public:
  huffman_decode_tree(const huffman_table& table)
    : root_{nullptr} {
    this->build_tree(table);
  }

  huffman_decode_tree_node* get_root() const {
    return this->root_.get();
  }

 private:
  void build_tree(const huffman_table& table) {
    this->root_ = std::make_unique<huffman_decode_tree_node>();
    for (const auto& item : table) {
      const auto symbol = item.first;
      const auto code = item.second;
      auto* node = this->root_.get();
      for (const auto& bit : code) {
        if (bit == '0') {
          if (node->get_left() == nullptr) {
            node->set_left(std::make_unique<huffman_decode_tree_node>());
          }
          node = node->get_left();
        } else if (bit == '1') {
          if (node->get_right() == nullptr) {
            node->set_right(std::make_unique<huffman_decode_tree_node>());
          }
          node = node->get_right();
        } else {
          assert(false);
        }
      }
      node->set_symbol(symbol);
    }
  }

 private:
  std::unique_ptr<huffman_decode_tree_node> root_;
};

huffman_decode_tree build_decode_tree(const huffman_table& table) {
  return huffman_decode_tree{table};
}

} // namespace

namespace huffman {

struct huffman_code {
  std::string encoded;
  std::unordered_map<char, std::string> table;
  std::size_t text_size;
};

template <typename Iter>
huffman_code encode(Iter first, Iter last) {
  const auto stats = build_symbol_stats(first, last);
  const auto tree = build_encode_tree(stats);
  const auto table = build_table(tree);
  auto text_size = std::size_t{0};
  auto encoded = std::string{};
  for (auto it = first; it != last; ++it) {
    const auto symbol = *it;
    auto here = table.find(symbol);
    assert(here != table.end());
    encoded += here->second;
    ++text_size;
  }
  return huffman_code{encoded, table, text_size};
}

huffman_code encode(const std::string& symbols) {
  return encode(std::begin(symbols), std::end(symbols));
}

std::pair<std::string, bool> decode(
  const std::string& encoded,
  const std::unordered_map<char, std::string>& table,
  std::size_t text_size) {
  auto tree = build_decode_tree(table);
  auto decoded = std::string{};
  auto* node = tree.get_root();
  for (const auto& bit : encoded) {
    if (node == nullptr) {
      return std::make_pair<std::string, bool>("", false);
    } else if (node->is_leaf()) {
      decoded += node->get_symbol();
      node = tree.get_root();
    } else {
      if (bit == '0') {
        node = node->get_left();
      } else if (bit == '1') {
          node = node->get_right();
      } else {
         assert(false);
      }
    }
  }
  if (decoded.size() != text_size) {
    return std::make_pair<std::string, bool>("", false);
  }
  return std::make_pair<std::string, bool>(std::move(decoded), true);
}

std::pair<std::string, bool> decode(const huffman_code& code) {
  return decode(code.encoded, code.table, code.text_size);
}

} // namespace huffman

int run() {
  std::ostream& out = std::cout;
  std::istream& in = std::cin;
  auto text = std::string{};
  in >> text;
  auto code = huffman::encode(text);
  out << code.table.size() << ' ' << code.encoded.size() << '\n';
  for (const auto& item : code.table) {
    out << item.first << ": " << item.second << '\n';
  }
  out << code.encoded;
  return 0;
}

int main()
{
    run();
    return 0;
}