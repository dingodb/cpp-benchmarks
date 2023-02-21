
#include <iostream>
#include <cstring>

#include <benchmark/benchmark.h>

static void DoSetup(const benchmark::State& state) {
}

static void DoTeardown(const benchmark::State& state) {
}

static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));

  for (auto _ : state)
    memcpy(dst, src, state.range(0));

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}
// BENCHMARK(BM_func)->Arg(1)->Arg(3)->Threads(16)->Threads(32)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_memcpy)
  ->Arg(8)
  ->Arg(64)
  ->Arg(512)
  ->Arg(4<<10)
  ->Arg(8<<10)
  ->Setup(DoSetup)->Teardown(DoTeardown);

// ----------------------------------------
void BM_spin_empty(benchmark::State& state) {
  for (auto _ : state) {
    for (int x = 0; x < state.range(0); ++x) {
      benchmark::DoNotOptimize(x);
    }
  }
}
BENCHMARK(BM_spin_empty)
  // ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
  //   std::cout << "v.size() = " << v.size() << std::endl;
  //   return *(std::max_element(std::begin(v), std::end(v)));
  // })
  ->Arg(512);

// ----------------------------------------
static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();

