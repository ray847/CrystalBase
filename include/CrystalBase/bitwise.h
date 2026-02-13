#ifndef CRYSTALBASE_BITWISE_H_
#define CRYSTALBASE_BITWISE_H_

#include <bitset> // std::bitset

namespace crystal {

using std::bitset;

template <size_t kNBits>
constexpr bitset<kNBits> operator -(const bitset<kNBits>& bits) {
  auto res = ~bits;
  bool carry = true;
  for (size_t i = 0; i < kNBits; ++i) {
    if (carry & res[i]) {
      carry = true;
      res[i] = false;
    } else if (carry | res[i]) {
      carry = false;
      res[i] = true;
    } else {
      carry = false;
      res[i] = false;
    }
  }
  return res;
}

auto negbit(const auto& bits) {
  return -bits;
}

template <size_t kNBits>
constexpr bitset<kNBits> lowbit(const bitset<kNBits>& bits) {
  return bits & -bits;
}

};

#endif