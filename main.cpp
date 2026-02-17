#include <iostream>

#include <CrystalBase/fixed_string.h>

int main() {
	const crystal::fixed_string string{"123"};
	std::cout << string << std::endl;
  return 0;
}