#ifndef DINGODB_ROCKSDB_TXN_WRAPPER_H_
#define DINGODB_ROCKSDB_TXN_WRAPPER_H_

#include <rocksdb/utilities/transaction_db.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>

namespace dingodb {

  class RocksTxnOper {
    public:
      static RocksTxnOper* get_instance(std::string db_path, std::string db_cfg_file, bool optimistic = false) {
        if(instance == nullptr) {
          std::unique_lock<std::mutex> mt(resource_mutex);
          if(instance == nullptr) {
            instance = new RocksTxnOper(db_path, db_cfg_file, optimistic);
          }   
        }   
        return instance;
      }
      ROCKSDB_NAMESPACE::TransactionDB* get_rocksdb();
      ROCKSDB_NAMESPACE::OptimisticTransactionDB* get_opt_rocksdb();
      ROCKSDB_NAMESPACE::DBOptions* get_options();
      void close_db();

    private:
      RocksTxnOper(std::string db_path, std::string db_cfg_file, bool optimistic = false);

	  ROCKSDB_NAMESPACE::TransactionDB* txn_db_;
	  ROCKSDB_NAMESPACE::OptimisticTransactionDB* opt_txn_db_;
      ROCKSDB_NAMESPACE::DBOptions db_options_;
      bool is_init_;
      inline static RocksTxnOper* instance = nullptr;
      inline static std::mutex resource_mutex;
  };
}

#endif // DINGODB_ROCKSDB_TXN_WRAPPER_H_
