#include <iostream>
#include <unordered_set>

#include <CrystalBase/fixed_string.h>

int main() {
	const crystal::fixed_string string{"123"};
	std::unordered_set<crystal::fixed_string<32>> set;
	std::cout << string << std::endl;
  return 0;
}