#ifndef CRYSTALBASE_SLOT_MAP_H_
#define CRYSTALBASE_SLOT_MAP_H_

#include <cstddef> // size_t

#include <limits> // std::numeric_limits::max()
#include <memory> // std::allocator
#include <vector> // std::vector
#include <memory_resource> // std::pmr

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
  T& At(size_t idx) {return arr_.at(idx).obj;}
  const T& At(size_t idx) const {return arr_.at(idx).obj;}
  T& operator[](size_t idx) {return arr_[idx].obj;}
  const T& operator[](size_t idx) const {return arr_[idx].obj;}
  /* Capacity */
  void Reserve(size_t n) {arr_.reserve(n);}
  size_t Capacity() const {return arr_.capacity();}
  /* Modifiers */
  void Clear() {
    arr_.clear();
    free_head_ = nullidx;
  }
  [[nodiscard]] size_t PushBack(const T& ele) {
    size_t idx = arr_.size();
    arr_.emplace_back(ele);
    return idx;
  }
  template<typename...Args>
  [[nodiscard]] size_t EmplaceBack(Args... args) {
    return PushBack(T{args...});
  }
  [[nodiscard]] size_t Insert(const T& ele) {
    if (free_head_ != nullidx) {
      size_t idx = free_head_;
      free_head_ = arr_[free_head_].nxt;
      arr_[idx] = ele;
      return idx;
    } else return PushBack(ele);
  }
  template<typename...Args>
  [[nodiscard]] size_t Emplace(Args... args) {
    return Insert(T{args...});
  }
  void Erase(size_t idx) {
    arr_[idx].nxt = free_head_;
    free_head_ = idx;
  }
private:
  union Ele {
    T obj;
    size_t nxt;
    Ele() = default;
    explicit Ele(const T obj) : obj(obj) {}
    Ele& operator=(const T other_obj) {
      obj = other_obj;
      return *this;
    }
  };
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
