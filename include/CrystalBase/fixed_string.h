#ifndef CRYSTALBASE_STRING_LITERAL_H_
#define CRYSTALBASE_STRING_LITERAL_H_

#include <wchar.h>

#include <array>
#include <string>
#include <type_traits>

namespace crystal {

using std::array, std::string;

/**
 * A fixed size string.
 *
 * This type is static in that it can be used as a template argument.
 * Use a ordinary C++ string literal to initialize this class.
 *
 * @tparam n The number of characters. This isn't meant to be manually set.
 * Rather use a ordinary C++ string literal for initialization for template
 * argument deduction.
 */
template <size_t n>
class fixed_string {
 public:
  array<char, n> data_;
  constexpr fixed_string(const char (&str)[n]) {
    std::copy_n(str, n, data_.data());
  }
  constexpr bool operator==(const fixed_string& other) const = default;
  operator string() const {
    return string{ data_.data(), n };
  }
};

/* Concept */
template <typename T>
struct is_fixed_string : std::false_type {};
template <size_t n>
struct is_fixed_string<fixed_string<n>> : std::true_type {};
template <typename T>
concept is_fixed_string_v = is_fixed_string<T>::value;

} // namespace crystal

#endif