#include <iostream>

#include <CrystalBase/error.h>

int main() {
  crystal::Error e{"123"};
  std::cout << e.msg << std::endl;
  return 0;
}