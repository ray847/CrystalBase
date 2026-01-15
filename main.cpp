#include <format>
#include <iostream>

#include "CrystalBase/stable_vector.h"
#include "CrystalBase/strict_index.h"

struct tmp {
  int val;

  tmp() : val(0) { std::cout << "Default constructed." << std::endl; }
  explicit tmp(int val) : val(val) {
    std::cout << std::format("Constructor {}.", val) << std::endl;
  }
  tmp(const tmp& other) : val(other.val) {
    std::cout << std::format("Copy Constructor {}.", val) << std::endl;
  }
  tmp(tmp&& other) noexcept : val(other.val) {
    // Optional: Reset other.val to indicate it was moved
    other.val = -1;
    std::cout << std::format("Move Constructor {}.", val) << std::endl;
  }
  ~tmp() { std::cout << std::format("Destroy {}.", val) << std::endl; }
};

int main() {
  // Note: Initializer lists always force copies because the elements inside are const.
  // However, vector reallocations inside will now use Moves.
  crystal::stable_vector<tmp> sv{tmp{0}, tmp{1}};

  std::cout << "\n--- Adding 2 ---" << std::endl;
  // Will now use Move Constructor
  auto new_idx_2 = sv.emplace_back(std::move(tmp{2}));

  std::cout << "\n--- Adding 3 ---" << std::endl;
  crystal::StrictIdx<tmp> new_idx_3 = sv.emplace_back(3);

  std::cout << "\n--- Erasing ---" << std::endl;
  sv.erase(1);

  std::cout << "\n--- Adding 4 ---" << std::endl;
  auto new_idx_4 = sv.emplace_back(std::move(tmp{4}));

  return 0;
}
