#include <iostream>
#include <random>
#include <pthread.h>
#include <vector>
#include <string.h>

namespace zharov
{
  struct Task
  {
    double r;
    size_t tests;
    size_t seed;
    size_t inside;
  };

  struct THGuard
  {
    std::vector< pthread_t >& ths;
    size_t& created;
    size_t& completed;

    ~THGuard()
    {
      for (size_t i = completed; i < created; ++i)
      {
        pthread_join(ths[i], nullptr);
      }
    }
  };

  double area(double r, size_t threads, size_t tests);
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
  if (tests < threads)
  {
    std::cerr << "Tests must be mire than threads";
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

double zharov::area(double r, size_t threads, size_t tests)
{
  size_t base = tests / threads;
  size_t rest = tests % threads;
  std::vector< pthread_t > ths(threads);
  std::vector< Task > tasks(threads);

  size_t created = 0, completed = 0;
  THGuard thg {ths, created, completed};
  for (; created < threads; ++created)
  {
    tasks[created] = {r, base + (created < rest ? 1 : 0), created, 0};
    int err = pthread_create(&ths[created], nullptr, worker, &tasks[created]);
    if (err != 0)
    {
      throw std::runtime_error(strerror(err));
    }
  }
  size_t inside = 0;
  for (size_t i = 0; i < created; ++i)
  {
    int err = pthread_join(ths[i], nullptr);
    if (err != 0)
    {
      throw std::runtime_error(strerror(err));
    }
    ++completed;
    inside = tasks[i].inside;
  }

  return (4 * r * r) * inside / tests;

}

void* zharov::worker(void* data)
{
  auto* task = static_cast<Task*>(data);
  task->inside = calc(task->r, task->tests, task->seed);
  return nullptr;
}
