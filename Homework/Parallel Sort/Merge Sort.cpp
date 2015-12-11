#include "stdafx.h"
#include "Merge Sort.hpp"

#include <algorithm>

using namespace cs477;

namespace {
  void intern_sort(std::vector<int>&, std::vector<int>&, const std::size_t&, const std::size_t&);
  void merge(std::vector<int>&, std::vector<int>&, const std::size_t&, const std::size_t&, const std::size_t&);

  void intern_sort(std::vector<int>& toSort, std::vector<int>& temp, const std::size_t& begin, const std::size_t& end) {
    std::size_t mid = (begin + end) / 2;

    if (mid - begin >= 10'000'000) {
      // Do one half of the work on annother thread, while continuing to use
      // the current thread
      future<void> splitWork = queue_work([&toSort, &temp, begin, end, mid] {
        ::intern_sort(toSort, temp, begin, mid);
      });
      ::intern_sort(toSort, temp, mid + 1, end);

      splitWork.wait();

      ::merge(toSort, temp, begin, mid + 1, end);
    } else {
      // Create adjusted iterators for std::sort
      auto beginIt = toSort.begin();
      std::advance(beginIt, begin);

      auto endIt = toSort.begin();
      std::advance(endIt, end + 1);

      std::sort(beginIt, endIt);
    }
  }

  void merge(std::vector<int>& toSort, std::vector<int>& temp, const std::size_t& cbegin, const std::size_t& cmid, const std::size_t& cend) {

    // Copy over old values
    for (std::size_t i = cbegin; i <= cend; ++i) {
      temp[i] = toSort[i];
    }

    // Create necissary modifiable indecies
    std::size_t begin = cbegin;
    std::size_t mid = cmid;

    // Perform merge step
    for (std::size_t cur = cbegin; cur <= cend; ++cur) {
      if (begin == cmid) {
        toSort[cur] = temp[mid++];
      } else if (mid > cend) {
        toSort[cur] = temp[begin++];
      } else {
        toSort[cur] = temp[begin] < temp[mid] ? temp[begin++] : temp[mid++];
      }
    }
  }
}

void sort(std::vector<int>& data) {
  std::vector<int> temp(data.size());

  ::intern_sort(data, temp, 0, data.size() - 1);
}
