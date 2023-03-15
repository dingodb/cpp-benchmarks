#ifndef DINGODB_ROCKSDB_WRAPPER_H_
#define DINGODB_ROCKSDB_WRAPPER_H_

#include <rocksdb/db.h>

namespace dingodb {

  class RocksOper {
    public:
      static RocksOper*  get_instance(std::string db_path, std::string db_cfg_file) {
        if(instance == nullptr) {
          std::unique_lock<std::mutex> mt(resource_mutex);
          if(instance == nullptr) {
            instance = new RocksOper(db_path, db_cfg_file);
          }   
        }   
        return instance;
      }
      ROCKSDB_NAMESPACE::DB* get_rocksdb();
      ROCKSDB_NAMESPACE::DBOptions* get_options();
      void close_db();

    private:
      RocksOper(std::string db_path, std::string db_cfg_file);

      ROCKSDB_NAMESPACE::DB* db;
      ROCKSDB_NAMESPACE::DBOptions db_options;
      std::string db_path;
      std::string db_cfg_file;
      bool is_init;
      inline static RocksOper* instance = nullptr;
      inline static std::mutex resource_mutex;
  };
}

#endif // DINGODB_ROCKSDB_WRAPPER_H_
