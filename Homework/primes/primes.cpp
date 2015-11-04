// primes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <vector>
#include <iostream>

#include "../../class-common/include/thread.h"

using namespace cs477;

bool is_prime(int n) {
  auto j = static_cast<int>(sqrt(n));
  for (int i = 2; i <= j; i++) {
    if (n % i == 0) return false;
  }
  return true;
}


int main() {
  const unsigned int threadCount = std::thread::hardware_concurrency();
  const unsigned int elements = 1'000'000;
  const unsigned int threadUnit = elements / threadCount;

  std::vector<future<void>> futures;
  std::vector<std::vector<int>> primesVec(threadCount);

  try {
    for (unsigned int i = 0; i < threadCount; ++i) {
      auto future = queue_work([i, &threadUnit, &primesVec] {
	      std::vector<int>& vec = primesVec[i];

	      for (unsigned int k = i * threadUnit; k < (i + 1) * threadUnit; ++k) {
          if (is_prime(k)) {
            vec.push_back(k);
          }
	      }
      });
      futures.push_back(std::move(future));
    }

    when_all(futures.begin(), futures.end());

    for (auto&& vec : primesVec) {
      for (auto&& entry : vec) {
        std::cout << entry << std::endl;
      }
    }
  } catch (std::system_error &ex) {
    printf("Error: %d (%s)\n", ex.code().value(), ex.what());
  } catch (std::exception &ex) {
    printf("Error: %s\n", ex.what());
  } catch (...) {
    printf("Error!\n");
  }

  return 0;
}
