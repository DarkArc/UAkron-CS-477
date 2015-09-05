// primes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <iostream>
#include "../../include/thread.h"

bool is_prime(int n)
{
  auto j = static_cast<int>(sqrt(n));
  for (int i = 2; i <= j; i++)
  {
    if (n % i == 0) return false;
  }
  return true;
}


int main()
{
  const int threadCount = 10000;
  const int elements = 1000000;
  const int threadUnit = elements / threadCount;

  std::vector<thread> threads;

  try {
    for (int i = 0; i < threadCount; ++i) {
      threads.push_back(
        create_thread([i, threadUnit]{
          for (int k = i * threadUnit; k < (i + 1) * threadUnit; ++k) {
              if (is_prime(k))
              {
                printf("%d\n", k);
              }
            }
          }
        )
      );
    }

    for (auto&& thread : threads)
    {
      join(thread);
    }
  }
  catch (std::system_error &ex)
  {
    printf("Error: %d (%s)\n", ex.code().value(), ex.what());
  }
  catch (std::exception &ex)
  {
    printf("Error: %s\n", ex.what());
  }
  catch (...)
  {
    printf("Error!\n");
  }

  return 0;
}
