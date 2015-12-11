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
  using FutureOfPrimes = future<std::vector<int>>;

  const unsigned int threadCount = std::thread::hardware_concurrency() * 5;
  const unsigned int elements = 1'000'000;
  const unsigned int threadUnit = elements / threadCount;

  std::vector<FutureOfPrimes> futures;

  try {
    for (unsigned int i = 0; i < threadCount; ++i) {
      auto future = queue_work([i, &threadUnit] {
	      std::vector<int> vec;

	      for (unsigned int k = i * threadUnit; k < (i + 1) * threadUnit; ++k) {
          if (is_prime(k)) {
            vec.push_back(k);
          }
	      }
        return vec;
      });
      futures.push_back(std::move(future));
    }

    future<std::vector<FutureOfPrimes>> result = when_all(futures.begin(), futures.end());
    
    for (auto&& future : result.get()) {
      for (auto&& primeNum : future.get()) {
        std::cout << primeNum << std::endl;
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
