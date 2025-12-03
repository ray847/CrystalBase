#ifndef CRYSTALBASE_CONCEPTS_H_
#define CRYSTALBASE_CONCEPTS_H_

#include <concepts>
#include <type_traits>

namespace crystal {
template<typename T, typename OutputType = void>
concept Callable = requires (T t) {
  {t()} -> std::convertible_to<OutputType>;
};
} // namespace crystal

#endif
