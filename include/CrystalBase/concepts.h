#ifndef CRYSTALBASE_CONCEPTS_H_
#define CRYSTALBASE_CONCEPTS_H_

#include <cstddef>

#include <array>
#include <concepts>
#include <type_traits>

namespace crystal {
template <typename T, typename OutputType = void>
concept Callable = requires(T t) {
  { t() } -> std::convertible_to<OutputType>;
};

template <typename T, size_t size>
concept ofSize = sizeof(T) == size;

template <typename T>
struct is_std_array : std::false_type {};
template <typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};
template <typename T>
concept is_std_array_v = is_std_array<T>::value;
} // namespace crystal

#endif