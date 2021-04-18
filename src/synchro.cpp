#include <fmt/format.h>

#include <version>

#ifdef __cpp_lib_latch

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <latch>
#include <thread>
#include <vector>

uint64_t collatz(uint64_t input)
{
   if (input % 2)
   {
      return 3 * input + 1;
   }
   else
   {
      return input / 2;
   }
}

struct Worker
{
   Worker(uint64_t startValue) : value{startValue}
   {
   }

   void run()
   {
      value = collatz(value);
      steps++;
   }

   int steps = 0;

   uint64_t value = 0;
};

std::stop_source          stopFlag;
std::vector<Worker>       workers;
std::vector<std::jthread> threads;

void start(int count)
{
   workers.reserve(count);
   threads.reserve(count);

   for (int ii = 0; ii < count; ++ii)
   {
      workers.emplace_back(std::chrono::steady_clock::now().time_since_epoch().count());
   }

   std::latch startFlag{count};

   for (int ii = 0; ii < count; ++ii)
   {
      auto& theWorker = workers[ii];
      threads.emplace_back(
         [&theWorker, &startFlag](std::stop_token st) {
            startFlag.arrive_and_wait();

            const auto id = std::hash<std::thread::id>()(std::this_thread::get_id());

            fmt::print("Thread {:x} started\n", id);
            fflush(stdout);

            while (!st.stop_requested())
            {
               theWorker.run();
            }

            fmt::print("Thread {:x} stopped\n", id);
         },
         stopFlag.get_token());
   }

   startFlag.wait();

   fmt::print("All {} threads started\n", count);
   fflush(stdout);
}

void stop()
{
   stopFlag.request_stop();

   fmt::print("All threads notified\n");

   threads.clear();

   fmt::print("All threads stopped\n");
}

constexpr int k_DefaultThreadCount = 8;

int main(int argc, char* argv[])
{
   int threadCount = k_DefaultThreadCount;
   if (argc > 1)
   {
      threadCount = atoi(argv[1]);

      if (0 == threadCount)
      {
         threadCount = k_DefaultThreadCount;
      }
   }

   start(threadCount);

   std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::seconds{5});

   stop();

   for (const auto& ww : workers)
   {
      fmt::print("{:>12} steps\n", ww.steps);
   }

   return 0;
}

#else

int main()
{
   fmt::print("Latch not supported\n");
   return 1;
}

#endif
