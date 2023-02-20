#include <cstdio>
#include <string>

#include <fmt/core.h>

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>

#include "utils.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteOptions;

const std::string kDBPath_for_get = "rocksdb_benchmark_get";

const int MAX_ITEMS = 100000000;

int main() {
  DB* db;
  Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  Status s = DB::Open(options, kDBPath_for_get, &db);
  assert(s.ok());

  for (int i = 0; i < MAX_ITEMS; i++) {
    std::string key = fmt::format("{:08}", i);
    std::string val = gen_random(100);

    s = db->Put(WriteOptions(), key, val);
    assert(s.ok());
  }

  delete db;

  return 0;
}
