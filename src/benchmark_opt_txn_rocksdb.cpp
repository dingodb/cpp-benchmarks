#include <algorithm>
#include <iostream>
#include <memory>
#include <fmt/core.h>
#include <rocksdb/options.h>
#include <benchmark/benchmark.h>
#include <rocksdb/utilities/transaction.h>

#include "rocksdb_txn_wrapper.h"
#include "utils.h"

using ROCKSDB_NAMESPACE::OptimisticTransactionDB;
using ROCKSDB_NAMESPACE::DBOptions;
using ROCKSDB_NAMESPACE::Iterator;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteOptions;
using ROCKSDB_NAMESPACE::Snapshot;
using ROCKSDB_NAMESPACE::Transaction;

const std::string kDBPath = "rocksdb_benchmark";
const std::string kDB_cfg_path = "./conf/conf.ini";
OptimisticTransactionDB* opt_txn_db;
dingodb::RocksTxnOper* rocks_oper;

/**
  * Setup and Teardown Empty Database for Testing Put
  */
static void DoSetup_for_EmptyDb(const benchmark::State& state) {
  rocks_oper = dingodb::RocksTxnOper::get_instance(
      kDBPath,
      kDB_cfg_path,
	  true);
  opt_txn_db = rocks_oper->get_opt_rocksdb();
  assert(nullptr != opt_txn_db);
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
      s = opt_txn_db->Put(WriteOptions(), key, val);
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
  rocks_oper = dingodb::RocksTxnOper::get_instance(
      kDBPath,
      kDB_cfg_path,
	  true);
  opt_txn_db = rocks_oper->get_opt_rocksdb();
  assert(opt_txn_db != nullptr);
}

static void DoTeardown_for_Get(const benchmark::State& state) {
  // Cleanup
}

static void BM_rocksdb_get(benchmark::State& state) {
  Status s;

  for (auto _ : state) {
    state.PauseTiming();
    int nKey = rand() % 100000;
    std::string key = fmt::format("{:08}", nKey);
    state.ResumeTiming();

    std::string val;
    s = opt_txn_db->Get(ReadOptions(), key, &val);
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
    rdOpts.snapshot = opt_txn_db->GetSnapshot();
    rdOpts.readahead_size = state.range(1);
    std::unique_ptr<Iterator> iter(opt_txn_db->NewIterator(rdOpts));
    iter->Seek(key);
    int count = 0;
    for (int i = 0; i < scanRange; i++) {
      std::string val;
      s = opt_txn_db->Get(rdOpts, key, &val);
      assert(s.ok());
      count++;
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

static void BM_release_resource(benchmark::State& state) {
  rocks_oper->close_db();
}

BENCHMARK(BM_release_resource)
  ->Iterations(1);

BENCHMARK_MAIN();
