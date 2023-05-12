#ifndef STORAGE_LEVELDB_DB_FILENAME_H_
#define STORAGE_LEVELDB_DB_FILENAME_H_

#include <string>
namespace leveldb
{

    // todo: 为什么这里需要使用 extern？
    extern std::string LogFileName(const std::string &dbname, uint64_t log_number);
}

#endif