#ifndef CRYSTALBASE_UNROLLED_FOR_LOOP_H_
#define CRYSTALBASE_UNROLLED_FOR_LOOP_H_

#include <cstddef> // size_t

#include "concepts.h"

namespace crystal {
template<Callable Op, size_t N_ITER>
void UnrolledForLoop(Op op = Op{}) {
  if constexpr (N_ITER) {
    op();
    UnrolledForLoop<Op, N_ITER - 1>(op);
  }
}
} // namespace crystal::base

#endif
