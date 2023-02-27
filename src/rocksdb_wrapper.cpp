#include "rocksdb_wrapper.h"
#include <iostream>
#include <rocksdb/options.h>
#include "rocksdb/utilities/options_util.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::DBOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::ConfigOptions;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;
using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;

namespace dingodb {


RocksOper::RocksOper(std::string db_path, std::string db_cfg_file) {
  ConfigOptions config_options;
  std::vector<ColumnFamilyDescriptor> cf_descs;
  Status s = LoadOptionsFromFile(config_options, db_cfg_file, &db_options, &cf_descs);
  if (!s.ok()) {
    std::cout << "Load Options Files failed." << std::endl;
  }

  std::vector<ColumnFamilyHandle*> handles;
  db_options.create_if_missing = true;
  s = DB::Open(db_options, db_path, cf_descs, &handles, &db);
  if (!s.ok()) {
    std::cout << "DB Open Failed." << std::endl;
  } else {
    is_init = true;
    std::cout << "DB Init Success:PTR" << db << ", " << std::endl;
  }
}

ROCKSDB_NAMESPACE::DB* RocksOper::get_rocksdb() {
   if (is_init) {
     return db;
   }
  std::cout << "DB get rocksdb Success:PTR" << db << ", " << std::endl;
   return nullptr;
}

ROCKSDB_NAMESPACE::DBOptions* RocksOper::get_options() {
  return &db_options;
}

void RocksOper::close_instance() {
  is_init = false;
  db->Close();
  delete db;
  db = nullptr;
}

}
