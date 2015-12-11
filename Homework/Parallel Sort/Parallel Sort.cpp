// Parallel Sort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Merge Sort.hpp"

void populate(std::vector<int>& vec) {
  std::minstd_rand numGen(std::chrono::system_clock::now().time_since_epoch().count());
  for (auto&& entry : vec) {
    entry = numGen() % 100;
  }
}

void print(const std::vector<int>& vec) {
  bool first = true;
  for (auto&& entry : vec) {
    if (!first) {
      std::cout << ", ";
    }
    std::cout << entry;
    first = false;
  }
  std::cout << std::endl;
}

int main()
{
  const int numToGen = 200'000'000;
  std::vector<int> toSort(numToGen);

  populate(toSort);
  std::cout << "Original" << std::endl;
  //print(toSort);

  cs477::sort(toSort.begin(), toSort.end());
  std::cout << "Final" << std::endl;
  //print(toSort);

  system("pause");

  return 0;
}

