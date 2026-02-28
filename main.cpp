#include <iostream>
#include <unordered_set>
#include <format>

#include <CrystalBase/fixed_string.h>

int main() {
	const crystal::fixed_string string{"123"};
	std::unordered_set<crystal::fixed_string<32>> set;
	std::cout << string << std::endl;
	std::cout << std::format("fixed string: {}", string) << std::endl;

  return 0;
}