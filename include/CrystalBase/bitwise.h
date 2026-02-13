#ifndef CRYSTALBASE_BITWISE_H_
#define CRYSTALBASE_BITWISE_H_

#include <bitset> // std::bitset
#include <concepts> // std::unsigned_integral

namespace crystal {

using std::bitset, std::unsigned_integral;

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

template <unsigned_integral u>
constexpr u lowbit(u bits) {
  return bits & -bits;
}

};

#endif