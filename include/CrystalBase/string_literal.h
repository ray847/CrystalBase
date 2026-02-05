#ifndef CRYSTALBASE_STRING_LITERAL_H_
#define CRYSTALBASE_STRING_LITERAL_H_

#include <wchar.h>

#include <array>
#include <string>
#include <type_traits>

namespace crystal {

using std::array, std::string;

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
  array<char, n> data_;
  constexpr string_literal(const char (&str)[n]) {
    std::copy_n(str, n, data_.data());
  }
  constexpr bool operator==(const string_literal& other) const = default;
  operator string() const {
    return string{ data_.data(), n };
  }
};

/* Concept */
template <typename T>
struct is_string_literal : std::false_type {};
template <size_t n>
struct is_string_literal<string_literal<n>> : std::true_type {};
template <typename T>
concept is_string_literal_v = is_string_literal<T>::value;

} // namespace crystal

#endif