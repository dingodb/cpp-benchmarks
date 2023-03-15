#include <iostream>
#include <rocksdb/options.h>
#include <rocksdb/utilities/options_util.h>

#include "rocksdb_txn_wrapper.h"

using ROCKSDB_NAMESPACE::TransactionDB;
using ROCKSDB_NAMESPACE::OptimisticTransactionDB;
using ROCKSDB_NAMESPACE::DBOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::ConfigOptions;
using ROCKSDB_NAMESPACE::TransactionDBOptions;
using ROCKSDB_NAMESPACE::OptimisticTransactionDBOptions;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;
using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;

namespace dingodb {

RocksTxnOper::RocksTxnOper(std::string db_path, std::string db_cfg_file, bool optimistic) {
  ConfigOptions config_options;
  std::vector<ColumnFamilyDescriptor> cf_descs;
  Status s = LoadOptionsFromFile(config_options, db_cfg_file, &db_options_, &cf_descs);
  if (!s.ok()) {
    std::cout << "ERROR, Load Options Files failed." << std::endl;
	return;
  }

  std::vector<ColumnFamilyHandle*> handles;
  db_options_.create_if_missing = true;
  if (optimistic) {
    OptimisticTransactionDBOptions occ_options;
    s = OptimisticTransactionDB::Open(db_options_, occ_options, db_path,
      cf_descs, &handles, &opt_txn_db_);
    txn_db_ = nullptr;
  } else {
    TransactionDBOptions txn_db_options;
    s = TransactionDB::Open(db_options_, txn_db_options, db_path,
      cf_descs, &handles, &txn_db_);
    opt_txn_db_ = nullptr;
  }
  if (!s.ok()) {
    is_init_ = false;
    std::cout << "ERROR, TXN DB Open Failed, status: " << s.ToString()
        << ", optimistic: " << optimistic << std::endl;
  } else {
    is_init_ = true;
    std::cout << "TXN DB Init Success:PTR, " << txn_db_ << " | " << opt_txn_db_
        << ", optimistic: " << optimistic << std::endl;
  }
}

ROCKSDB_NAMESPACE::TransactionDB* RocksTxnOper::get_rocksdb() {
  if (is_init_) {
    return txn_db_;
  }
  std::cout << "ERROR, TXN DB get rocksdb failed:PTR, " << txn_db_ << ", " << std::endl;
  return nullptr;
}

ROCKSDB_NAMESPACE::OptimisticTransactionDB* RocksTxnOper::get_opt_rocksdb() {
  if (is_init_) {
    return opt_txn_db_;
  }
  std::cout << "ERROR, OPTIMISTIC TXN DB get rocksdb Failed:PTR, " << opt_txn_db_
      << ", " << std::endl;
  return nullptr;
}

ROCKSDB_NAMESPACE::DBOptions* RocksTxnOper::get_options() {
  return &db_options_;
}

void RocksTxnOper::close_db() {
  is_init_ = false;
  if(txn_db_ != nullptr) {
    delete txn_db_;
    txn_db_ = nullptr;
  }

  if(opt_txn_db_ != nullptr) {
    delete opt_txn_db_;
    opt_txn_db_ = nullptr;
  }
}

}
