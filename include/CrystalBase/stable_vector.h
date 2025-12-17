#ifndef CRYSTALBASE_STABLE_VECTOR_H_
#define CRYSTALBASE_STABLE_VECTOR_H_

#include <cstddef>

#include <vector>
#include <list>
#include <memory_resource>
#include <utility>
#include <memory>

namespace crystal {

template<typename T, typename Alloc = std::allocator<T>>
class StableVector {
public:

  using allocator_type = Alloc;

  /* Constructors */
  StableVector() : StableVector(allocator_type{}) {}

  explicit StableVector(const allocator_type& allocator):
    arr_{allocator},
    free_list_{allocator} {}

  template<typename Iter>
  StableVector(Iter begin, Iter end, const allocator_type& allocator = {}):
    StableVector(allocator) {
    while (begin != end) {
      (void)PushBack(*begin++);
    }
  }

  StableVector(std::initializer_list<T> lst,
               const allocator_type& allocator = {}):
    StableVector(lst.begin(), lst.end(), allocator) {}

  StableVector(const StableVector& other,
               const allocator_type& allocator = {}):
    arr_{other.arr_, allocator},
    free_list_{other.free_list_, allocator} {}

  StableVector(StableVector&&) = default;

  StableVector(StableVector&& other, const allocator_type& allocator):
    arr_{std::move(other.arr_), allocator},
    free_list_{std::move(other.free_list_), allocator} {}

  /* Assignment Operators */
  StableVector& operator=(const StableVector& other) = default;
  StableVector& operator=(StableVector&& other) = default;
  ~StableVector() = default;

  allocator_type get_allocator() const { return arr_.get_allocator(); }

  /* Element Access */
  T& At(size_t idx) { return arr_.at(idx); }
  const T& At(size_t idx) const { return arr_.at(idx); }
  T& operator[](size_t idx) { return arr_[idx]; }
  const T& operator[](size_t idx) const { return arr_[idx]; }

  /* Capacity */
  void Reserve(size_t n) { arr_.reserve(n); }
  size_t Capacity() const { return arr_.capacity(); }

  /* Modifiers */
  void Clear() {
    arr_.clear();
    free_list_.clear();
  }

  [[nodiscard]] size_t PushBack(const T& ele) {
    arr_.emplace_back(ele);
    return arr_.size() - 1;
  }

  [[nodiscard]] size_t PushBack(T&& ele) {
    arr_.emplace_back(std::move(ele));
    return arr_.size() - 1;
  }

  template<typename... Args>
  [[nodiscard]] size_t EmplaceBack(Args&&... args) {
    arr_.emplace_back(std::forward<Args>(args)...);
    return arr_.size() - 1;
  }

 [[nodiscard]] size_t Insert(const T& ele) {
    if (!free_list_.empty()) {
      size_t idx = free_list_.front();
      free_list_.pop_front();
      arr_.emplace(arr_.begin() + idx, ele);
      return idx;
    } else return PushBack(ele);
  }

  [[nodiscard]] size_t Insert(T&& ele) {
    if (!free_list_.empty()) {
      size_t idx = free_list_.front();
      free_list_.pop_front();
      arr_.emplace(arr_.begin() + idx, ele);
      return idx;
    } else return PushBack(std::move(ele));
  }

  template<typename...Args>
  [[nodiscard]] size_t Emplace(Args&&... args) {
    if (!free_list_.empty()) {
      size_t idx = free_list_.front();
      free_list_.pop_front();
      arr_.emplace(arr_.begin() + idx, args...);
      return idx;
    } else {
      return EmplaceBack(args...);
    }
  }
  void Erase(size_t idx) {
    arr_[idx].~T();
    free_list_.push_front(idx);
  }

private:
  std::vector<
  T,
  typename std::allocator_traits<Alloc>::template rebind_alloc<T>
  > arr_;
  std::list<
  size_t,
  typename std::allocator_traits<Alloc>::template rebind_alloc<size_t>
  > free_list_;
};

namespace pmr {
template <typename T>
using StableVector = StableVector<T, std::pmr::polymorphic_allocator<T>>;
} // namespace pmr
} // namespace crystal

#endif
