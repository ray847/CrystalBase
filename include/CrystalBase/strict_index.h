#ifndef CRYSTALBASE_STRICT_INDEX_H_
#define CRYSTALBASE_STRICT_INDEX_H_

#include <limits> // std::numeric_limits
#include <functional> // std::hash
#include <concepts> // std::integral

namespace crystal {
template <typename T, std::integral Idx = size_t>
class StrictIdx {
public:
  static constexpr Idx nullvalue = std::numeric_limits<Idx>::max();
  /* Constructor */
  constexpr StrictIdx(Idx v = nullvalue) : value(v) {} // NOLINT
  /* Functions */
  constexpr Idx get() const { return value; }
  /* Operators */
  auto operator<=>(const StrictIdx&) const = default;
  constexpr StrictIdx& operator++() {
    ++value;
    return *this;
  }
  constexpr StrictIdx operator++(int) {
    StrictIdx temp = *this;
    ++value;
    return temp;
  }
  explicit operator bool() const {return value == nullvalue;}
  operator Idx() const {return value;} // NOLINT
private:
  Idx value;
};
} // namespace crystal

/* Hashing for std::unordered_set & std::unordered_map. */
namespace std {
template <typename Idxag, typename Idx>
struct hash<crystal::StrictIdx<Idxag, Idx>> {
  size_t operator()(const crystal::StrictIdx<Idxag, Idx>& idx) const {
    return hash<Idx>()(idx.get());
  }
};
}

#endif
