#include <iostream>
#include <bitset>

#include <CrystalBase/bitwise.h>

int main() {
	std::bitset<16> bits = 0b0101010;
	auto neg = crystal::negbit(bits);
	auto lowbit = crystal::lowbit(bits);
	std::cout << bits << ' ' << neg << ' ' <<  lowbit << std::endl;
  return 0;
}