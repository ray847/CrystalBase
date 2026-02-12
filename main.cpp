#include <iostream>

#include <CrystalBase/integer_sequence.h>

template <crystal::integer_sequence seq>
void Print() {
	for (auto ele : seq) {
		std::cout << ele << ' ';
	}
	std::cout << std::endl;
}

int main() {
	Print<{1, 2, 3}>();
  return 0;
}