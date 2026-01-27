#ifndef CRYSTALBASE_STABLE_VECTOR_H_
#define CRYSTALBASE_STABLE_VECTOR_H_

#include <cstddef>
#include <memory>
#include <memory_resource>
#include <utility>
#include <variant>
#include <vector> // std::vector

namespace crystal {

template <typename T, typename Alloc = std::allocator<T>>
class stable_vector {
 public:
  using allocator_type = Alloc; // allocator aware type

  /* Constructors */
  stable_vector() : stable_vector(allocator_type{}) {
  }
  explicit stable_vector(const allocator_type& allocator) : arr_{ allocator } {
  }
  template <typename Iter>
  stable_vector(Iter begin, Iter end, const allocator_type& allocator = {}) :
      stable_vector(allocator) {
    while (begin != end) { (void)push_back(*begin++); }
  }
  stable_vector(std::initializer_list<T> lst,
                const allocator_type& allocator = {}) :
      stable_vector(lst.begin(), lst.end(), allocator) {
  }
  stable_vector(const stable_vector& other,
                const allocator_type& allocator = {}) :
      arr_{ other.arr_, allocator }, free_head_{ other.free_head_ } {
  }
  stable_vector(stable_vector&&) = default;
  stable_vector(stable_vector&& other, const allocator_type& allocator) :
      arr_{ std::move(other.arr_), allocator }, free_head_{ other.free_head_ } {
  }

  /* Assignment Operators */
  stable_vector& operator=(const stable_vector& other) = default;
  stable_vector& operator=(stable_vector&& other) = default;

  /* Destructor */
  ~stable_vector() = default;

  allocator_type get_allocator() const {
    return arr_.get_allocator();
  }

  /* Element Access */
  T& at(size_t idx) {
    return std::get<T>(arr_.at(idx));
  }
  const T& at(size_t idx) const {
    return std::get<T>(arr_.at(idx));
  }
  T& operator[](size_t idx) {
    return std::get<T>(arr_[idx]);
  }
  const T& operator[](size_t idx) const {
    return std::get<T>(arr_[idx]);
  }

  /* Capacity */
  void reserve(size_t n) {
    arr_.reserve(n);
  }
  size_t capacity() const {
    return arr_.capacity();
  }

  /* Modifiers */
  void clear() {
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
   * check `insert`.
   */
  [[nodiscard]] size_t push_back(const T& ele) {
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
   * check `insert`.
   */
  [[nodiscard]] size_t push_back(T&& ele) {
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
   * check `insert`.
   */
  template <typename... Args>
  [[nodiscard]] size_t emplace_back(Args&&... args) {
    arr_.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
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
  [[nodiscard]] size_t insert(const T& ele) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<size_t>(arr_[free_head_]);
      arr_[idx].template emplace<T>(ele);
      return idx;
    } else return push_back(ele); // fall back
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
  [[nodiscard]] size_t insert(T&& ele) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<size_t>(arr_[free_head_]);
      arr_[idx].template emplace<T>(ele);
      return idx;
    } else return push_back(ele); // fall back
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
  template <typename... Args>
  [[nodiscard]] size_t emplace(Args&&... args) {
    if (free_head_ != kNullIdx) {
      size_t idx = free_head_;
      free_head_ = std::get<size_t>(arr_[free_head_]);
      arr_[idx].template emplace<T>(args...);
      return idx;
    } else {
      return emplace_back(args...);
    }
  }
  void erase(size_t idx) {
    // arr_[idx].~T();
    arr_[idx] = free_head_;
    free_head_ = idx;
  }

 private:
  using ele = std::variant<T, size_t>;
  static constexpr size_t kNullIdx = -1ul;
  /* Variables */
  std::vector<ele,
              typename std::allocator_traits<Alloc>::template rebind_alloc<ele>>
      arr_;
  size_t free_head_ = kNullIdx;
};

namespace pmr {
template <typename T>
using stable_vector = stable_vector<T, std::pmr::polymorphic_allocator<T>>;
} // namespace pmr
} // namespace crystal

#endif