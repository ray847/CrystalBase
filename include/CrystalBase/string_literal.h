#ifndef CRYSTALBASE_STRING_LITERAL_H_
#define CRYSTALBASE_STRING_LITERAL_H_

#include <array>

namespace crystal {

using std::array;

/**
 * A literal string.
 *
 * This type is literal in that it can be used as a template argument.
 * Use a ordinary C++ string literal to initialize this class.
 *
 * @tparam n The number of characters. This isn't meant to be manually set.
 * Rather use a ordinary C++ string literal for initialization for template
 * argument deduction.
 */
template <size_t n>
class string_literal {
 public:
  constexpr string_literal(const char(&str)[n]) {
    std::copy_n(str, n, data_.data());
  }
  constexpr bool operator==(const string_literal& other) const = default;
 private:
  array<char, n> data_;
};

} // namespace crystal

#endif