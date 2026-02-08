#include <string>
#include <iostream>

#include "include/CrystalBase/fixed_string.h"
#include "CrystalBase/static_format.h"

int main() {
  using std::string;
  //auto fs = crystal::static_format<-123>();
  auto fs = crystal::static_format<"{}def{}", crystal::fixed_string("abc"), 456>();
  std::cout << string(fs);
}
