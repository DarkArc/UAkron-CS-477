#pragma once

#include "stdafx.h"

#include "../../class-common/include/thread.h"

#include <vector>
#include <algorithm>

namespace cs477 {
  namespace internal_sort {
    template <typename ItType>
      void sort(const ItType&, std::vector<int>&, const ItType&, const ItType&);

    template <typename ItType>
      void merge(const ItType&, std::vector<int>&, const ItType&, const ItType&, const ItType&);

    template <typename ItType>
      void sort(const ItType& trueBegin, std::vector<int>& temp, const ItType& begin, const ItType& end) {
        auto mid = std::next(begin, std::distance(begin, end) / 2);

        if (std::distance(begin, mid) >= 10'000'000) {
          // Do one half of the work on annother thread, while continuing to use
          // the current thread
          future<void> splitWork = queue_work([&trueBegin, &temp, begin, end, mid] {
            sort(trueBegin, temp, begin, mid);
          });
          sort(trueBegin, temp, std::next(mid), end);

          splitWork.wait();

          merge(trueBegin, temp, begin, std::next(mid), end);
        } else {
          std::sort(begin, end);
        }
      }

    template <typename ItType>
      void merge(const ItType& trueBegin, std::vector<int>& temp, const ItType& cbegin, const ItType& cmid, const ItType& cend) {

        // Create two temporary iterators
        // one for the beginning of the modified temp indicie, one for the mid
        auto tempBegin = std::next(temp.begin(), std::distance(trueBegin, cbegin));
        auto tempMid = std::next(temp.begin(), std::distance(trueBegin, cmid));

        // Copy over the data from the source vector into the temporary vector
        std::copy(cbegin, cend, tempBegin);

        // Perform merge step
        for (auto cur = cbegin; cur != cend; ++cur) {
          // We're comparing iterators of two different vectors, so we must derive the offsets
          // for comparison
          if (std::distance(temp.begin(), tempBegin) == std::distance(trueBegin, cmid)) {
            *cur = *(tempMid++);
          } else if (std::distance(temp.begin(), tempMid) > std::distance(trueBegin, cend)) {
            *cur = *(tempBegin++);
          } else {
            *cur = *tempBegin < *tempMid ? *(tempBegin++) : *(tempMid++);
          }
        }
      }
  }

  template<typename ItType>
    void sort(ItType start, ItType end) {
      // Perform a one time allocation equal in size to the input vector
      // this prevents repeated copies which slow down the algorithm
      std::vector<int> temp(std::distance(start, end));

      cs477::internal_sort::sort(start, temp, start, end);
    }
}

