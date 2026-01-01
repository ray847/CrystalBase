#ifndef CRYSTALBASE_STABLE_VECTOR_H_
#define CRYSTALBASE_STABLE_VECTOR_H_

#include <cstddef>

#include <vector> // std::vector
#include <memory_resource>
#include <utility>
#include <memory>
#include <variant>

namespace crystal {

template<typename T, typename Alloc = std::allocator<T>>
class StableVector {
public:
  using allocator_type = Alloc; // allocator aware type

  /* Constructors */
  StableVector() : StableVector(allocator_type{}) {}
  explicit StableVector(const allocator_type& allocator):
    arr_{allocator},
    free_head_{free_head_} {}
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
    free_head_{free_head_} {}
  StableVector(StableVector&&) = default;
  StableVector(StableVector&& other, const allocator_type& allocator):
    arr_{std::move(other.arr_), allocator},
    free_head_{std::move(other.free_head_), allocator} {}

  /* Assignment Operators */
  StableVector& operator=(const StableVector& other) = default;
  StableVector& operator=(StableVector&& other) = default;

  /* Destructor */
  ~StableVector() = default;

  allocator_type get_allocator() const {return arr_.get_allocator();}

  /* Element Access */
  T& At(size_t idx) { return std::get<T>(arr_.at(idx)); }
  const T& At(size_t idx) const { return std::get<T>(arr_.at(idx)); }
  T& operator[](size_t idx) { return std::get<T>(arr_[idx]); }
  const T& operator[](size_t idx) const { return std::get<T>(arr_[idx]); }

  /* Capacity */
  void Reserve(size_t n) { arr_.reserve(n); }
  size_t Capacity() const { return arr_.capacity(); }

  /* Modifiers */
  void Clear() {
    arr_.clear();
    free_head_ = kNullIdx;
  }
  /**
   * Push a new element to the back of the container.
   *
   * @param ele Element to insert.
   * @return size_t Index to the inserted element.
   *
   * @note This function does not check if there are vacant slots in the
   * underlying vector. For inserting elements with vacant slot utilizaiton,
   * check `Insert`.
   */
  [[nodiscard]] size_t PushBack(const T& ele) {
    arr_.push_back(ele);
    return arr_.size() - 1;
  }
  /**
   * Push a new element to the back of the container.
   *
   * @param ele Element to insert.
   * @return size_t Index to the inserted element.
   *
   * @note This function does not check if there are vacant slots in the
   * underlying vector. For inserting elements with vacant slot utilizaiton,
   * check `Insert`.
   */
  [[nodiscard]] size_t PushBack(T&& ele) {
    arr_.push_back(std::move(ele));
    return arr_.size() - 1;
  }
  /**
   * Construct a new element to the back of the container.
   *
   * @param args Arguments for constructing the element.
   * @return size_t Index to the inserted element.
   *
   * @note This function does not check if there are vacant slots in the
   * underlying vector. For inserting elements with vacant slot utilizaiton,
   * check `Insert`.
   */
  template<typename... Args>
  [[nodiscard]] size_t EmplaceBack(Args&&... args) {
    arr_.push_back(Ele{}.emplace(std::forward<Args>(args)...));
    return arr_.size() - 1;
  }
  /**
   * Insert a new element into the container.
   *
   * @param ele Element to insert.
   * @return size_t Index to the inserted element.
   *
   * @note This function checks for vacant slots for insert first. Then it falls
   * back to appending the underlying vector. For direct appending, check
   * `PushBack`.
   */
 [[nodiscard]] size_t Insert(const T& ele) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<size_t>(arr_[free_head_]);
      arr_[idx].template emplace<T>(ele);
      return idx;
    } else return PushBack(ele); // fall back
  }
  /**
   * Insert a new element into the container.
   *
   * @param ele Element to insert.
   * @return size_t Index to the inserted element.
   *
   * @note This function checks for vacant slots for insert first. Then it falls
   * back to appending the underlying vector. For direct appending, check
   * `PushBack`.
   */
  [[nodiscard]] size_t Insert(T&& ele) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<size_t>(arr_[free_head_]);
      arr_.template emplace<T>(arr_.begin() + idx, ele);
      return idx;
    } else return PushBack(ele); // fall back
  }
  /**
   * Construct a new element in the container.
   *
   * @param args Arguments for constructing the element.
   * @return size_t Index to the inserted element.
   *
   * @note This function checks for vacant slots for insert first. Then it falls
   * back to appending the underlying vector. For direct appending, check
   * `PushBack`.
   */
  template<typename...Args>
  [[nodiscard]] size_t Emplace(Args&&... args) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<T>(arr_[free_head_]);
      arr_[idx].template emplace<T>(args...);
      return idx;
    } else {
      return EmplaceBack(args...);
    }
  }
  void Erase(size_t idx) {
    //arr_[idx].~T();
    arr_[idx] = free_head_;
    free_head_ = idx;
  }

private:
  using Ele = std::variant<T, size_t>;
  static constexpr size_t kNullIdx = -1ul;
  /* Variables */
  std::vector<
  Ele,
  typename std::allocator_traits<Alloc>::template rebind_alloc<Ele>
  > arr_;
  size_t free_head_ = kNullIdx;
};

namespace pmr {
template <typename T>
using StableVector = StableVector<T, std::pmr::polymorphic_allocator<T>>;
} // namespace pmr
} // namespace crystal

#endif
