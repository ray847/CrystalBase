#include "CrystalBase/strict_index.h"
#include "CrystalBase/stable_vector.h"

int main() {
  crystal::StableVector<int> sv{0, 1};
  auto new_idx_2 = sv.PushBack(2);
  crystal::StrictIdx<int> new_idx_3{sv.PushBack(3)};
  sv.Erase(1);
  auto new_idx_4 = sv.PushBack(4);
  auto new_idx_5 = sv.Emplace(5);
  return 0;
}
