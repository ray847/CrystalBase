#ifndef CRYSTALBASE_ERROR_H_
#define CRYSTALBASE_ERROR_H_

#include <stacktrace>
#include <string>

namespace crystal {

struct Error {
  std::string msg;
  std::stacktrace stacktrace;
  Error(std::string msg,
        std::stacktrace stacktrace = std::stacktrace::current()) :
      msg(msg), stacktrace(stacktrace) {
  }
};

} // namespace crystal::base

#endif
