#ifndef CRYSTALBASE_DBLIST_H_
#define CRYSTALBASE_DBLIST_H_

#include <stdint.h>
#include <cstddef>

#include <iterator>
#include <list>
#include <memory>
#include <vector>

namespace crystal {
/**
 * A dynamic blocked list.
 *
 * Each block can be of any dynamic size.
 */
template <typename T, typename Allocator>
class dblist {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = std::allocator_traits<Allocator>::pointer;
  using const_pointer = std::allocator_traits<Allocator>::const_pointer;
  class iterator;
  class const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 private:
  using block_type = std::vector<T, Allocator>;
  using list_type =
      std::list<block_type, typename std::allocator_traits<
                                Allocator>::template rebind_alloc<block_type>>;
  list_type list_;
  size_t size_;
};

template <typename T, typename Allocator>
class dblist<T, Allocator>::iterator {
  static_assert(std::bidirectional_iterator<iterator>);
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  /* Constructor */
  iterator(decltype(list_type{}.begin()) block_iter,
           size_t inner_idx) = default;
  iterator(const iterator& other) = default;

  /* Assignment Operator */
  iterator& operator=(const iterator& rhs) = default;

  /* Iterator Operator */
  value_type& operator*() const { return (*block_iter_)[inner_idx_]; }
  iterator& operator++() {
    if (inner_idx_ < block_iter_->size() - 1) {
      inner_idx_++;
    } else {
      block_iter_++;
      inner_idx_ = 0;
    }
    return *this;
  }
  iterator operator++(iterator) {
    auto tmp = *this;
    if (inner_idx_ < block_iter_->size() - 1) {
      inner_idx_++;
    } else {
      block_iter_++;
      inner_idx_ = 0;
    }
    return tmp;
  }
  iterator& operator--() {
    if (inner_idx_ > 0) {
      inner_idx_--;
    } else {
      block_iter_--;
      inner_idx_ = block_iter_->size() - 1;
    }
    return *this;
  }
  iterator operator--(iterator) {
    auto tmp = *this;
    if (inner_idx_ > 0) {
      inner_idx_--;
    } else {
      block_iter_--;
      inner_idx_ = block_iter_->size() - 1;
    }
    return tmp;
  }
  bool operator==(const iterator& other) const {
    return block_iter_ == other.block_iter_ && inner_idx_ == other.inner_idx_;
  }

 private:
  decltype(list_type{}.begin()) block_iter_;
  size_t inner_idx_;
};

template <typename T, typename Allocator>
class dblist<T, Allocator>::const_iterator {
  static_assert(std::bidirectional_iterator<const_iterator>);
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  /* Constructor */
  const_iterator(decltype(list_type{}.cbegin()) block_iter,
                 size_t inner_idx) = default;
  const_iterator(const iterator& other) = default;

  /* Assignment Operator */
  const_iterator& operator=(const iterator& rhs) = default;

  /* Iterator Operator */
  const value_type& operator*() const { return (*block_iter_)[inner_idx_]; }
  iterator& operator++() {
    if (inner_idx_ < block_iter_->size() - 1) {
      inner_idx_++;
    } else {
      block_iter_++;
      inner_idx_ = 0;
    }
    return *this;
  }
  iterator operator++(iterator) {
    auto tmp = *this;
    if (inner_idx_ < block_iter_->size() - 1) {
      inner_idx_++;
    } else {
      block_iter_++;
      inner_idx_ = 0;
    }
    return tmp;
  }
  iterator& operator--() {
    if (inner_idx_ > 0) {
      inner_idx_--;
    } else {
      block_iter_--;
      inner_idx_ = block_iter_->size() - 1;
    }
    return *this;
  }
  iterator operator--(iterator) {
    auto tmp = *this;
    if (inner_idx_ > 0) {
      inner_idx_--;
    } else {
      block_iter_--;
      inner_idx_ = block_iter_->size() - 1;
    }
    return tmp;
  }
  bool operator==(const iterator& other) const {
    return block_iter_ == other.block_iter_ && inner_idx_ == other.inner_idx_;
  }

 private:
  decltype(list_type{}.cbegin()) block_iter_;
  size_t inner_idx_;
};
} // namespace crystal

#endif