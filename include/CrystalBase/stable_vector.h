#ifndef CRYSTALBASE_SLOT_MAP_H_
#define CRYSTALBASE_SLOT_MAP_H_

#include <cstddef> // size_t

#include <limits> // std::numeric_limits::max()
#include <memory> // std::allocator
#include <vector> // std::vector
#include <memory_resource> // std::pmr
#include <variant> // std::variant
#include <utility> // std::move, std::forward

namespace crystal {
template<typename T, typename Allocator = std::allocator<T>>
class StableVector {
public:
  using allocator_type = Allocator;
  /* Constructor */
  StableVector() : StableVector(Allocator{}) {}
  explicit StableVector(const Allocator& allocator):
    arr_{allocator}, free_head_(nullidx) {}
  template<typename Iter>
  StableVector(Iter begin, Iter end, const Allocator& allocator = {}):
    StableVector(allocator) {
    while (begin != end) {
      (void)PushBack(*begin++); // neglecting return value
    }
  }
  StableVector(std::initializer_list<T> lst, const Allocator& allocator = {}):
    StableVector(lst.begin(), lst.end(), allocator) {}
  StableVector(const StableVector& other,
               const Allocator& allocator = {}):
    arr_{other.arr_, allocator}, free_head_(other.free_head_) {}
  StableVector(StableVector&&) = default;
  StableVector(StableVector&& other, allocator_type allocator):
    arr_{std::move(other.arr_), allocator}, free_head_(other.free_head_) {
      other.free_head_ = nullidx;
    }
  /* Assignment Operator */
  StableVector& operator=(const StableVector& other) = default;
  StableVector& operator=(StableVector&& other) = default;
  ~StableVector() = default;
  allocator_type get_allocator() const {return arr_.get_allocator();}
  /* Functions */
  /* Element Access */
  T& At(size_t idx) {return std::get<T>(arr_.at(idx));}
  const T& At(size_t idx) const {return std::get<T>(arr_.at(idx));}
  T& operator[](size_t idx) {return std::get<T>(arr_[idx]);}
  const T& operator[](size_t idx) const {return std::get<T>(arr_[idx]);}
  /* Capacity */
  void Reserve(size_t n) {arr_.reserve(n);}
  size_t Capacity() const {return arr_.capacity();}
  /* Modifiers */
  void Clear() {
    arr_.clear();
    free_head_ = nullidx;
  }
  [[nodiscard]] size_t PushBack(const T& ele) {
    arr_.emplace_back(std::in_place_type<T>, ele);
    return arr_.size() - 1;
  }
  [[nodiscard]] size_t PushBack(T&& ele) {
    arr_.emplace_back(std::in_place_type<T>, std::move(ele));
    return arr_.size() - 1;
  }
  template<typename...Args>
  [[nodiscard]] size_t EmplaceBack(Args&&... args) {
    arr_.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
    return arr_.size() - 1;
  }
  [[nodiscard]] size_t Insert(const T& ele) {
    if (free_head_ != nullidx) {
      size_t idx = free_head_;
      // Retrieve the next free index from the variant
      free_head_ = std::get<size_t>(arr_[free_head_]);
      // Assign the new element to the variant
      arr_[idx].template emplace<T>(ele);
      return idx;
    } else return PushBack(ele);
  }
  [[nodiscard]] size_t Insert(T&& ele) {
    if (free_head_ != nullidx) {
      size_t idx = free_head_;
      // Retrieve the next free index from the variant
      free_head_ = std::get<size_t>(arr_[free_head_]);
      // Assign the new element to the variant
      arr_[idx].template emplace<T>(std::move(ele));
      return idx;
    } else return PushBack(std::move(ele));
  }
  template<typename...Args>
  [[nodiscard]] size_t Emplace(Args&&... args) {
    if (free_head_ != nullidx) {
      size_t idx = free_head_;
      // Retrieve the next free index from the variant
      free_head_ = std::get<size_t>(arr_[free_head_]);
      // Construct the new element in-place within the variant
      arr_[idx].template emplace<T>(std::forward<Args>(args)...);
      return idx;
    } else {
      // If no free slots, push to back
      arr_.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
      return arr_.size() - 1;
    }
  }
  void Erase(size_t idx) {
    arr_[idx].template emplace<size_t>(free_head_);
    free_head_ = idx;
  }
private:
  using Ele = std::variant<T, size_t>; // Changed to std::variant
  using EleAllocator =
  typename std::allocator_traits<Allocator>::template rebind_alloc<Ele>;
  std::vector<Ele, EleAllocator> arr_;
  size_t free_head_;
  static constexpr size_t nullidx = std::numeric_limits<size_t>::max();
};

namespace pmr {
template<typename T>
using StableVector = StableVector<T, std::pmr::polymorphic_allocator<>>;
} // namespace pmr

} // namespace crystal

#endif
