
#include <rocksdb/db.h>

namespace dingodb {

  class RocksOper {
    public:
      static RocksOper*  get_instance(std::string db_path, std::string db_cfg_file) {
        static RocksOper instance(db_path, db_cfg_file);
        return &instance;
      }

      ROCKSDB_NAMESPACE::DB* get_rocksdb();
      ROCKSDB_NAMESPACE::DBOptions* get_options();
      void close_instance();

    private:
      RocksOper(std::string db_path, std::string db_cfg_file);
      ROCKSDB_NAMESPACE::DB* db;
      ROCKSDB_NAMESPACE::DBOptions db_options;
      std::string db_path;
      std::string db_cfg_file;
      bool is_init;
  };
}
