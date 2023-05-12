#ifndef STORAGE_LEVELDB_INCLUDE_DB_H_
#define STORAGE_LEVELDB_INCLUDE_DB_H_

#include <string>
#include "leveldb/status.h"
#include "leveldb/options.h"

namespace leveldb
{
    class DB
    {
    public:
        DB(){};
        virtual ~DB();
        /// 使用案例
        /// Db* db;
        /// Status status = leveldb::DB::Open(options, "/tmp/testdb", &db_);
        static Status Open(const Options &options,
                           const std::string &name,
                           DB **dbptr);
    };

}
#endif