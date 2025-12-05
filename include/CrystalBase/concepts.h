#ifndef CRYSTALBASE_CONCEPTS_H_
#define CRYSTALBASE_CONCEPTS_H_

#include <cstddef> // size_t

#include <concepts>
#include <type_traits> // std::convertible_to

namespace crystal {
template<typename T, typename OutputType = void>
concept Callable = requires (T t) {
  {t()} -> std::convertible_to<OutputType>;
};

template<typename T, size_t size>
concept ofSize = sizeof(T) == size;
} // namespace crystal

#endif
