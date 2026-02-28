#ifndef CRYSTALBASE_FIXED_STRING_H_
#define CRYSTALBASE_FIXED_STRING_H_

#include <algorithm>
#include <array>
#include <ostream>
#include <string_view>

namespace crystal {

/**
 * A fixed size string.
 *
 * This type is static in that it can be used as a template argument.
 * Use a ordinary C++ string literal to initialize this class. The null
 * terminator is STRIPPED during construction.
 *
 * @tparam n The number of characters. This isn't meant to be manually set.
 * Rather use a ordinary C++ string literal for initialization for template
 * argument deduction.
 */
template <size_t N>
struct fixed_string {
  std::array<char, N> data_{};

  constexpr fixed_string() = default;

  constexpr fixed_string(const char (&str)[N + 1]) {
    std::copy_n(str, N, data_.begin());
  }

  constexpr explicit fixed_string(std::array<char, N> const& arr) : data_(arr) {
  }

  constexpr operator std::string_view() const {
    return std::string_view(data_.data(), N);
  }

  constexpr size_t size() const {
    return N;
  }
  constexpr char operator[](size_t i) const {
    return data_[i];
  }
  constexpr bool operator==(const fixed_string& other) const = default;
};

/* Output to stream. */
template <size_t kN>
std::ostream& operator<<(std::ostream& os, const fixed_string<kN>& fs) {
  return os << static_cast<std::string_view>(fs);
}

/* Template deduction guide for construction from string literal. */
template <size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N - 1>;

/* Concept */
template <typename T>
struct is_fixed_string : std::false_type {};
template <size_t n>
struct is_fixed_string<fixed_string<n>> : std::true_type {};
template <typename T>
concept is_fixed_string_v = is_fixed_string<T>::value;

/* Utility Functions */
template <auto... fs>
consteval auto join() {
  constexpr size_t total_len = (fs.size() + ... + 0);
  std::array<char, total_len> arr{};

  size_t offset = 0;
  auto append = [&](auto const& s) {
    for (size_t i = 0; i < s.size(); ++i) arr[offset++] = s[i];
  };
  (append(fs), ...);

  return fixed_string<total_len>(arr);
}

template <fixed_string s, size_t st, size_t ed>
consteval auto slice() {
  constexpr size_t new_len = ed - st;
  std::array<char, new_len> arr{};
  for (size_t i = 0; i < new_len; ++i) { arr[i] = s[st + i]; }
  return fixed_string<new_len>(arr);
}

template <fixed_string text, fixed_string pattern>
consteval size_t find_idx() {
  if (pattern.size() > text.size()) return -1;
  // <= because we are strictly comparing content (no nulls)
  for (size_t i = 0; i + pattern.size() <= text.size(); ++i) {
    bool match = true;
    for (size_t j = 0; j < pattern.size(); ++j) {
      if (text[i + j] != pattern[j]) {
        match = false;
        break;
      }
    }
    if (match) return i;
  }
  return -1;
}

} // namespace crystal

namespace std {
/* Hashing */
template <size_t n>
struct hash<crystal::fixed_string<n>> {
  size_t operator()(const crystal::fixed_string<n>& str) const noexcept {
    size_t hash = 0;
    for (size_t i = 0; i < n; ++i) hash ^= str[i];
    return hash;
  }
};
/* Formatting */
template <size_t kN>
struct formatter<crystal::fixed_string<kN>> : formatter<string> {
  constexpr auto parse(format_parse_context& ctx) {
    return formatter<string>::parse(ctx);
  }

  auto format(const crystal::fixed_string<kN>& fs, format_context& ctx) const {
    std::string str{std::string_view{fs}};
    return formatter<std::string>::format(str, ctx);
  }
};
} // namespace std

#endif