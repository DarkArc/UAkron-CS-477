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
  using future_of_primes = future<std::vector<int>>;

  const unsigned int thread_count = std::thread::hardware_concurrency() * 5;
  const unsigned int elements = 1'000'000;
  const unsigned int thread_unit = elements / thread_count;

  std::vector<future_of_primes> futures;

  try {
    for (unsigned int i = 0; i < thread_count; ++i) {
      auto future = queue_work([i, &thread_unit] {
	      std::vector<int> vec(thread_unit);

	      for (unsigned int k = i * thread_unit; k < (i + 1) * thread_unit; ++k) {
          if (is_prime(k)) {
            vec.push_back(k);
          }
	      }
        return vec;
      });
      futures.push_back(std::move(future));
    }

    future<std::vector<future_of_primes>> result = when_all(futures.begin(), futures.end());
    
    for (auto&& future : result.get()) {
      for (auto&& prime_num : future.get()) {
        std::cout << prime_num << std::endl;
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
