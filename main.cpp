#include <iostream>
#include <random>
#include <pthread.h>
#include <vector>

namespace zharov
{
  struct Task
  {
    double r;
    size_t tests;
    size_t seed;
    size_t inside;
  };

  double area(double r, size_t threds, size_t tests);
  size_t calc(double r, size_t tests, size_t seed);
  bool isInside(double x, double y, double r);
  void* worker(void* data);
}

int main(int argc, char** argv)
{
  size_t threads = 4;
  if (argc > 1)
  {
    threads = std::stoull(argv[1]);
  }

  double r;
  size_t tests;
  std::cin >> r >> tests;
  if (r <= 0 || tests == 0 || threads == 0) {
    std::cerr << "Args must be positive\n";
    return 1;
  }

  try
  {
    std::cout << zharov::area(r, threads, tests) << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
