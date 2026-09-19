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
{}
