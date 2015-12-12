#pragma once

#include <array>

#define PRIMES_QUEUE_SIZE 1
#define PRIMES_PER_PROCESS 10

namespace cs477 {
  using shared_prime_request = unsigned int;

  template<unsigned int N>
    using shared_prime_reply = std::array<unsigned int, N>;
}
