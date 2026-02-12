#ifndef CRYSTALBASE_INTEGER_SEQUENCE_H_
#define CRYSTALBASE_INTEGER_SEQUENCE_H_

namespace crystal {

/**
 * An integer sequence that can be initialized with a `{1, 2, 3, ...}` syntax.
 */
template <size_t kN>
struct integer_sequence {
  /* Varaibles */
  size_t data[kN];
  /* Constructor */
  template <typename... Ts>
  constexpr integer_sequence(Ts... args) :
      data{ static_cast<size_t>(args)... } {
  }
  /* Iterators for ranges based for loops. */
  constexpr const size_t* begin() const {
    return data;
  }
  constexpr const size_t* end() const {
    return data + kN;
  }
  /* Accessors */
  constexpr size_t size() const {
    return kN;
  }
  constexpr size_t operator[](size_t idx) const {
    return data[idx];
  }
};

/* Deduction Guide */
template <typename... Ts>
integer_sequence(Ts...) -> integer_sequence<sizeof...(Ts)>;

} // namespace crystal

#endif