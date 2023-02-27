#include <cstdio>
#include <string>
#include <iostream>

#include <fmt/core.h>

#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include "rocksdb_wrapper.h"

#include "utils.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteOptions;

const std::string kDBPath_for_get = "rocksdb_benchmark";
const std::string kDBOptions_config_file = "./conf/conf.ini";

const long MAX_ITEMS = 200000000;

void printUsage() {
    std::cout << "Usage:\n\n \t\t ./prepare_rocksdb recordCnt dbPath cfgPath" << std::endl;
    return ;
}

int main(int argc, char* argv[]) {
  long record_cnt = MAX_ITEMS;
  std::string db_path = kDBPath_for_get.c_str();
  std::string db_cfg_file = kDBOptions_config_file.c_str();

  if (argc != 1 && argc != 4) {
    printUsage();
    return -1;
  }

  if (argc == 4) {
    record_cnt = atol(argv[1]);
    db_path = argv[2];
    db_cfg_file = argv[3];
  }

  std::cout << "RecordCnt:" << record_cnt << ", DbPath:" << db_path << ", Cfg config:" << db_cfg_file << std::endl;

  dingodb::RocksOper* rocks_oper = dingodb::RocksOper::get_instance(db_path, db_cfg_file);
  if (nullptr == rocks_oper) {
    std::cout << "Init RocksOper Failed!" << std::endl;
    return -1;
  }

  DB* db = rocks_oper->get_rocksdb();
  if (nullptr == db) {
    std::cout << "Init RocksDB Instance Failed" << std::endl;
    return -1;
  }

  Status s;
  for (long i = 0; i < record_cnt; i++) {
    std::string key = fmt::format("{:08}", i);
    std::string val = gen_random(100);
    s = db->Put(WriteOptions(), key, val);
    assert(s.ok());
  }
  rocks_oper->close_instance();
  return 0;
}
