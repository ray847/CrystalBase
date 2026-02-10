#ifndef CRYSTALBASE_STATIC_FORMAT_H_
#define CRYSTALBASE_STATIC_FORMAT_H_

#include <concepts>
#include <type_traits>
#include <array>

#include "fixed_string.h"


namespace crystal {

template <auto val>
consteval auto static_format_arg() {
  if constexpr (std::is_same_v<decltype(val), char>) {
    return fixed_string<1>(std::array<char, 1>{val});
  } else if constexpr (std::integral<decltype(val)>) {
    if constexpr (val == 0) return fixed_string("0");

    // Calculate digits
    auto get_len = [](auto v) {
      size_t l = 0;
      if (v < 0) {
        l++;
        v = -v;
      }
      while (v > 0) {
        l++;
        v /= 10;
      }
      return l;
    };
    constexpr size_t N = get_len(val);

    std::array<char, N> arr{};
    auto tmp = val;
    bool neg = tmp < 0;
    if (neg) tmp = -tmp;

    size_t i = N;
    while (tmp > 0) {
      arr[--i] = '0' + (tmp % 10);
      tmp /= 10;
    }
    if (neg) arr[0] = '-';

    return fixed_string<N>(arr);
  } else if constexpr (is_fixed_string_v<decltype(val)>) {
    return val;
  } else {
    static_assert(false, "Input type not support for static formatting.");
  }
}

template <fixed_string format_str, fixed_string prefix, auto arg, auto... args>
consteval auto static_format_impl() {
  constexpr size_t end = find_idx<format_str, "{}">();
  static_assert(end != size_t(-1), "Missing '{}' placeholder for argument.");

  if constexpr (sizeof...(args) > 0) {
    return static_format_impl<
        slice<format_str, end + 2, format_str.size()>(), // New Tail
        join<prefix,
             slice<format_str, 0, end>(),
             static_format_arg<arg>()>(), // New Prefix
        args...
    >();
  } else {
    /* End of Recursion */
    return join<prefix,
                slice<format_str, 0, end>(),
                static_format_arg<arg>(),
                slice<format_str, end + 2, format_str.size()>()>();
  }
}

template <fixed_string format_string, auto... args>
consteval auto static_format() {
  if constexpr (sizeof...(args) == 0) {
    return format_string;
  } else {
    return static_format_impl<format_string, fixed_string(""), args...>();
  }
}

} // namespace crystal

#endif