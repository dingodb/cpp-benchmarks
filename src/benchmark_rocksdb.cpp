
#include <algorithm>
#include <iostream>
#include <memory>

#include <fmt/core.h>

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>

#include <benchmark/benchmark.h>
#include "rocksdb_wrapper.h"

#include "utils.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::DBOptions;
using ROCKSDB_NAMESPACE::Iterator;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteOptions;

const std::string kDBPath = "rocksdb_benchmark";
const std::string kDB_cfg_path = "./conf/conf.ini";
DB* db;
dingodb::RocksOper* rocks_oper;

/**
  * Setup and Teardown Empty Database for Testing Put
  */
static void DoSetup_for_EmptyDb(const benchmark::State& state) {
  rocks_oper = dingodb::RocksOper::get_instance(
      kDBPath,
      kDB_cfg_path);
  db = rocks_oper->get_rocksdb();
  assert(nullptr != db);
}

static void DoTeardown_for_EmptyDb(const benchmark::State& state) {
  // Cleanup
}

static void BM_rocksdb_put(benchmark::State& state) {
  Status s;

  for (auto _ : state) {
      state.PauseTiming();
      std::string key = gen_random(state.range(0));
      std::string val = gen_random(state.range(1));

      state.ResumeTiming();
      // Put single key-value
      s = db->Put(WriteOptions(), key, val);
      assert(s.ok());
  }
}
BENCHMARK(BM_rocksdb_put)
  ->ArgsProduct({
                  {10,  100,   1000,   10000}, // Key Size
                  {10, 1000, 100000, 1000000}  // Value Size
    })
  ->ThreadRange(1, 4)
  ->Setup(DoSetup_for_EmptyDb)->Teardown(DoTeardown_for_EmptyDb);
 
/**
  * Setup and Teardown Empty Database for Testing Put
  */
static void DoSetup_for_Get(const benchmark::State& state) {
  rocks_oper = dingodb::RocksOper::get_instance(
      kDBPath,
      kDB_cfg_path);
  db = rocks_oper->get_rocksdb();
  assert(db != nullptr);
}

static void DoTeardown_for_Get(const benchmark::State& state) {
}

static void BM_rocksdb_get(benchmark::State& state) {
  Status s;

  for (auto _ : state) {
    state.PauseTiming();
    int nKey = rand() % 100000;
    std::string key = fmt::format("{:08}", nKey);
    state.ResumeTiming();

    std::string val;
    s = db->Get(ReadOptions(), key, &val);
    assert(s.ok());
  }
}
BENCHMARK(BM_rocksdb_get)
  ->Args(
    {10,  100,   1000,   10000} // Key Size
  )
  ->Setup(DoSetup_for_Get)->Teardown(DoTeardown_for_Get);

static void BM_rocksdb_scan(benchmark::State& state) {
  Status s;

  for (auto _ : state) {
    state.PauseTiming();

    int nKey = rand() % 10000;
    std::string key = fmt::format("{:08}", nKey);

    state.ResumeTiming();

    int scanRange = state.range(0);
    ReadOptions rdOpts;
    rdOpts.readahead_size = state.range(1);
    std::unique_ptr<Iterator> iter(db->NewIterator(rdOpts));
    iter->Seek(key);
    size_t count = 0;
    for (int i = 0; i < scanRange; i++) {
      std::string val;
      s = db->Get(rdOpts, key, &val);
      assert(s.ok());

      count ++;
    }
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_rocksdb_scan)
  ->ArgsProduct({
                  benchmark::CreateDenseRange(128, 1024, 128), // Scan Count
                  benchmark::CreateRange(0, 102400, 10240)     // Read-Ahead Size
  })
  ->Setup(DoSetup_for_Get)->Teardown(DoTeardown_for_Get);

BENCHMARK_MAIN();
