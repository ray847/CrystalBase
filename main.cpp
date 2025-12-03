#include <iostream>

#include "CrystalBase/unrolled_for_loop.h"

int main() {
  struct Op1 {
    void operator()() {
      std::cout << "op1 called" << std::endl;
    }
  };
  auto op2 = [](){
    std::cout << "op2 called" << std::endl;
  };
  crystal::UnrolledForLoop<Op1, 3>();
  crystal::UnrolledForLoop<decltype(op2), 4>(op2);
  return 0;
}
