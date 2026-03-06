#ifndef CRYSTALBASE_FILE_IO_H_
#define CRYSTALBASE_FILE_IO_H_

#include <cassert>

#include <filesystem>
#include <fstream>
#include <string>
#include <expected>

#include "error.h"

namespace crystal {

inline std::expected<std::string, Error> ReadFile(std::filesystem::path file_path) {
  assert(file_path.has_filename() && "Input path does not point to a file");
  std::ifstream is{ file_path };
  if (!is.good()) return std::unexpected(Error{"Cannot open file."});
  std::string content;
  is >> content;
  return content;
}

} // namespace crystal

#endif
