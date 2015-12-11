#pragma once

#include "stdafx.h"

#include "../../class-common/include/thread.h"

#include <vector>
#include <algorithm>

namespace cs477 {
  template <typename ItType>
    void sort(const ItType& begin, const ItType& end) {
      auto mid = std::next(begin, std::distance(begin, end) / 2);

      if (std::distance(begin, mid) >= 10'00) {
        // Do one half of the work on annother thread, while continuing to use
        // the current thread
        future<void> splitWork = queue_work([begin, mid] {
          cs477::sort(begin, mid);
        });
        cs477::sort(mid, end);

        splitWork.wait();

        std::inplace_merge(begin, mid, end);
      } else {
        std::sort(begin, end);
      }
    }
}

